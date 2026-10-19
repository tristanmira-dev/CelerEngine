#include "uploadmanager.hpp"
#include "commonTypes.hpp"


namespace Celer {

	namespace Core {
		

		UploadManager::UploadManager(VulkanContext &vulkanContext) : mTransferCommandBuff(*vulkanContext.device, 1, vulkanContext.transferQueueIdx), mGraphicsCommandBuff(*vulkanContext.device, 1, vulkanContext.graphicsQueueIdx), mSharedStagingBuffer(KILO * KILO * 100, vulkanContext) {
			mUploadFence = vk::raii::Fence(*vulkanContext.device, vk::FenceCreateInfo{ .flags = vk::FenceCreateFlagBits::eSignaled });
		}


		void UploadManager::upload(FrameContext &frameCtx, VulkanContext &ctx) {

			if (mPendingUpload.size()) {

				//Catch things mid frame
				if (frameCtx.mTimelineCount != frameCtx.mFrameSyncObject.mSemaphore.getCounterValue()) {
					std::cout << "UNSYNCED!!! " << frameCtx.mTimelineCount << " " << frameCtx.mFrameSyncObject.mSemaphore.getCounterValue() << '\n';

					vk::SemaphoreWaitInfo waitInfo{ .semaphoreCount = 1, .pSemaphores = &*frameCtx.mFrameSyncObject.mSemaphore, .pValues = &frameCtx.mTimelineCount };

					ctx.device->waitSemaphores(waitInfo, UINT64_MAX);

					std::cout << "SYNCED??? " << frameCtx.mTimelineCount << " " << frameCtx.mFrameSyncObject.mSemaphore.getCounterValue() << "\n\n";

				}

				mTransferCommandBuff.getSingleBuffer().reset();

				mTransferCommandBuff.beginSingleTimeCommand();

				for (std::unique_ptr<Resource::UploadableResource>& uploads : mPendingUpload) {
					uploads->upload(mTransferCommandBuff, ctx);
				}

				mTransferCommandBuff.getSingleBuffer().end();

				frameCtx.mUploadCount++;

				vk::SemaphoreSubmitInfo timeline{
					.semaphore = *frameCtx.mUpload,
					.value = frameCtx.mUploadCount,
					.stageMask = vk::PipelineStageFlagBits2::eTransfer
				};

				std::array<vk::SemaphoreSubmitInfo, 1> semaphoreInfos{ timeline };


				vk::CommandBufferSubmitInfo commandBuffInfo{
					.commandBuffer = *mTransferCommandBuff.getSingleBuffer()
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

				//TODO BATCH PIPELINE BARRIERS (cuz pipelineBarrier has an overhead cost)

				for (std::unique_ptr<Resource::OwnershipTransfer>& acquireInfo : mPendingAcquire) {

					acquireInfo->acquire(mGraphicsCommandBuff, ctx);

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

		void UploadManager::queueUploadResource(std::unique_ptr<Resource::UploadableResource>&& uploadable) {
			mPendingUpload.push_back(std::move(uploadable));		
		}

		

	}


}