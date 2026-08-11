#include "uploadmanager.hpp"


namespace Celer {

	namespace Core {
		



		UploadManager::UploadManager(VulkanContext &vulkanContext) : mCommandBuff(*vulkanContext.device, 1, vulkanContext.transferQueueIdx) {
		
		}

		void UploadManager::update(FrameContext& frameCtx, VulkanContext& ctx, DeviceMemoryManager& memoryManager) {


			if (mPendingUpload.size()) {

				if (frameCtx.mTimelineCount != frameCtx.mFrameSyncObject.mSemaphore.getCounterValue()) {
					std::cout << "UNSYNCED!!! " << frameCtx.mTimelineCount << " " << frameCtx.mFrameSyncObject.mSemaphore.getCounterValue() << '\n';



					vk::SemaphoreWaitInfo waitInfo{ .semaphoreCount = 1, .pSemaphores = &*frameCtx.mFrameSyncObject.mSemaphore, .pValues = &frameCtx.mTimelineCount };

					ctx.device->waitSemaphores(waitInfo, UINT64_MAX);

					std::cout << "SYNCED??? " << frameCtx.mTimelineCount << " " << frameCtx.mFrameSyncObject.mSemaphore.getCounterValue() << "\n\n";

				}


				/*LOOP THROUGH PENDING UPLOAD*/
				

				for (ResourceUploadInfo& uploads : mPendingUpload) {
					switch (uploads.mResourceType) {
						case ResourceType::IMAGE: {
							//mCommandBuff.beginSingleTimeCommand();

							vk::raii::CommandBuffer &commandBuff{ mCommandBuff.getSingleBuffer() };

							transitionLayout(mCommandBuff, *uploads.mImageResource, vk::ImageLayout::eUndefined, vk::ImageLayout::eTransferDstOptimal);

							/*
								transition image first from undefined to dst optimal layout
								actual copy from host visible to local device happens here
								transition image to sample optimal
							
							
							*/

							transitionLayout(mCommandBuff, *uploads.mImageResource, vk::ImageLayout::eTransferDstOptimal, vk::ImageLayout::eShaderReadOnlyOptimal);

						}	
							

						break;
						case ResourceType::BUFFER:

							break;
						default:
							break;
					}
				}

			} 
		
		}

		void UploadManager::addImageResource(VulkanContext& vulkanCtx, ResourceType resourceType, Memory memoryInfo, void* data, vk::raii::Image *image) {
			switch (resourceType) {
				case ResourceType::IMAGE:
					mPendingUpload.emplace_back(resourceType, memoryInfo, data, vulkanCtx, image, nullptr);
					break;
				case ResourceType::BUFFER:
					
					break;
				default:
					break;

			}
		
		
		}

		ResourceUploadInfo::ResourceUploadInfo(ResourceType resourceType, Memory memory, void* data, VulkanContext& ctx, vk::raii::Image* image, vk::raii::Buffer* buffer) : mResourceType{ resourceType }, mMemoryInfo{ memory }, mBuffer(data, memory.getSize(), ctx), mImageResource{ image }, mBufferResource{ buffer } {
			
		
		}

	}


}