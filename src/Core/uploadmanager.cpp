#include "uploadmanager.hpp"


namespace Celer {

	namespace Core {
		

		void copyBufferToImage(Wrapper::CommandBuffer &commandBuffer, ResourceUploadInfo &uploadInfo) {
			vk::BufferImageCopy region{ .bufferOffset = 0,
											   .bufferRowLength = 0,
											   .bufferImageHeight = 0,
											   .imageSubresource = {.aspectMask = vk::ImageAspectFlagBits::eColor, .mipLevel = 0, .baseArrayLayer = 0, .layerCount = 1},
											   .imageOffset = {0, 0, 0},
											   .imageExtent = {uploadInfo.mWidth, uploadInfo.mHeight, 1
			} };

			commandBuffer.getSingleBuffer().copyBufferToImage(uploadInfo.mBuffer.getUnderlyingBuffer(), *uploadInfo.mImageResource, vk::ImageLayout::eTransferDstOptimal, region);
		
		}


		UploadManager::UploadManager(VulkanContext &vulkanContext) : mCommandBuff(*vulkanContext.device, 1, vulkanContext.transferQueueIdx), mGraphicsCommandBuff(*vulkanContext.device, 1, vulkanContext.graphicsQueueIdx) {
			mUploadFence = vk::raii::Fence(*vulkanContext.device, vk::FenceCreateInfo{ .flags = vk::FenceCreateFlagBits::eSignaled });
		}

		void UploadManager::addAcquire(ResourceAcquireInfo const &info) {

			mPendingAcquire.push_back(info);

		}

		void UploadManager::upload(FrameContext &frameCtx, VulkanContext &ctx) {

			if (mPendingUpload.size()) {

				if (frameCtx.mTimelineCount != frameCtx.mFrameSyncObject.mSemaphore.getCounterValue()) {
					std::cout << "UNSYNCED!!! " << frameCtx.mTimelineCount << " " << frameCtx.mFrameSyncObject.mSemaphore.getCounterValue() << '\n';

					vk::SemaphoreWaitInfo waitInfo{ .semaphoreCount = 1, .pSemaphores = &*frameCtx.mFrameSyncObject.mSemaphore, .pValues = &frameCtx.mTimelineCount };

					ctx.device->waitSemaphores(waitInfo, UINT64_MAX);

					std::cout << "SYNCED??? " << frameCtx.mTimelineCount << " " << frameCtx.mFrameSyncObject.mSemaphore.getCounterValue() << "\n\n";

				}

				mCommandBuff.getSingleBuffer().reset();

				mCommandBuff.beginSingleTimeCommand();



				for (ResourceUploadInfo& uploads : mPendingUpload) {

					switch (uploads.mResourceType) {

					case ResourceType::IMAGE: {
						//reset command buffer first....

						vk::raii::CommandBuffer& commandBuff{ mCommandBuff.getSingleBuffer() };

						transitionLayout(mCommandBuff, *uploads.mImageResource, vk::ImageLayout::eUndefined, vk::ImageLayout::eTransferDstOptimal);

						copyBufferToImage(mCommandBuff, uploads);

						releaseBarrier(mCommandBuff, *uploads.mImageResource, ctx.transferQueueIdx, ctx.graphicsQueueIdx);

						addAcquire(ResourceAcquireInfo{
							.oldQueue = ctx.transferQueueIdx,
							.newQueue = ctx.graphicsQueueIdx,
							.mImageResource = uploads.mImageResource
						}); //ADD PROPERTY TO SPECIFY WHICH QUEUE

					}

						break;

					case ResourceType::BUFFER:

						//todo u animal

						break;

					default:
						break;
					}
				}

				mCommandBuff.getSingleBuffer().end();

				frameCtx.mUploadCount++;

				vk::SemaphoreSubmitInfo timeline{
					.semaphore = *frameCtx.mUpload,
					.value = frameCtx.mUploadCount,
					.stageMask = vk::PipelineStageFlagBits2::eTransfer
				};

				std::array<vk::SemaphoreSubmitInfo, 1> semaphoreInfos{ timeline };


				vk::CommandBufferSubmitInfo commandBuffInfo{
					.commandBuffer = *mCommandBuff.getSingleBuffer()
				};

				const vk::SubmitInfo2 submitInfo{
					.flags = {},
					.commandBufferInfoCount = 1, .pCommandBufferInfos = &commandBuffInfo,
					.signalSemaphoreInfoCount = 1, .pSignalSemaphoreInfos = semaphoreInfos.data()

				};

				ctx.transferQueue->submit2(submitInfo, {});

				vk::SemaphoreWaitInfo semaWait{ .semaphoreCount = 1, .pSemaphores = &*frameCtx.mUpload, .pValues = &frameCtx.mUploadCount };

				//wait on the same upload lol no choice
				ctx.device->waitSemaphores(semaWait, UINT64_MAX);

				mPendingUpload.clear();

			}

		}

		void UploadManager::acquire(FrameContext& frameCtx, VulkanContext& ctx) {


			if (!mPendingAcquire.empty()) {

				mGraphicsCommandBuff.resetSingleBuff();
				mGraphicsCommandBuff.beginSingleTimeCommand();

				//TODO BATCH PIPELINE BARRIERS

				for (ResourceAcquireInfo& acquireInfo : mPendingAcquire) {


					vk::ImageMemoryBarrier imgMemory{
						.srcAccessMask = vk::AccessFlagBits::eNone,
						.dstAccessMask = vk::AccessFlagBits::eShaderRead,
						.oldLayout = vk::ImageLayout::eTransferDstOptimal,
						.newLayout = vk::ImageLayout::eShaderReadOnlyOptimal,
						.srcQueueFamilyIndex = ctx.transferQueueIdx,
						.dstQueueFamilyIndex = ctx.graphicsQueueIdx,
						.image = **acquireInfo.mImageResource,
					};

					imgMemory.subresourceRange = { .aspectMask = vk::ImageAspectFlagBits::eColor, .levelCount = 1, .layerCount = 1 };

					mGraphicsCommandBuff.getSingleBuffer().pipelineBarrier(vk::PipelineStageFlagBits::eTopOfPipe, vk::PipelineStageFlagBits::eFragmentShader, {}, {}, {}, imgMemory);


				}

				mPendingAcquire.clear();

				mGraphicsCommandBuff.getSingleBuffer().end();

				ctx.graphicsQueue->submit(vk::SubmitInfo{ .commandBufferCount = 1, .pCommandBuffers = &*mGraphicsCommandBuff.getSingleBuffer() });
			}
			
		}

		void UploadManager::update(FrameContext& frameCtx, VulkanContext& ctx, DeviceMemoryManager& memoryManager) {

			upload(frameCtx, ctx);

			acquire(frameCtx, ctx);

		}

		void UploadManager::addImageResource(VulkanContext& vulkanCtx, ResourceType resourceType, Memory memoryInfo, void* data, vk::raii::Image *image, uint32_t width, uint32_t height) {
			switch (resourceType) {
				case ResourceType::IMAGE:
					
					mPendingUpload.emplace_back(resourceType, memoryInfo, data, vulkanCtx, image, nullptr, width, height);
					break;

				case ResourceType::BUFFER:
					
					break;

				default:

					break;

			}
		
		
		}

		ResourceUploadInfo::ResourceUploadInfo(ResourceType resourceType, Memory memory, void* data, VulkanContext& ctx, vk::raii::Image* image, vk::raii::Buffer* buffer, uint32_t width, uint32_t height) : mResourceType{ resourceType }, mMemoryInfo{ memory }, mBuffer(data, memory.getSize(), ctx), mImageResource{ image }, mBufferResource{ buffer }, mWidth{ width }, mHeight{ height } {
			
		
		}

	}


}