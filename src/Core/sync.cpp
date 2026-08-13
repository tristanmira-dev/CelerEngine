#include "sync.hpp"


namespace Celer {

	namespace Core {
		void releaseBarrier(Celer::Wrapper::CommandBuffer& commandBuff, vk::raii::Image& img, uint32_t oldQueue, uint32_t newQueue) {
			vk::ImageMemoryBarrier imageBarrier{ .oldLayout=vk::ImageLayout::eTransferDstOptimal ,.newLayout = vk::ImageLayout::eShaderReadOnlyOptimal, .image = img };
			imageBarrier.srcQueueFamilyIndex = oldQueue;
			imageBarrier.dstQueueFamilyIndex = newQueue;
			imageBarrier.subresourceRange = { .aspectMask = vk::ImageAspectFlagBits::eColor, .levelCount = 1, .layerCount = 1 };

			imageBarrier.srcAccessMask = vk::AccessFlagBits::eTransferWrite;
			imageBarrier.dstAccessMask = vk::AccessFlagBits::eNone;

			commandBuff.getSingleBuffer().pipelineBarrier(vk::PipelineStageFlagBits::eTransfer, vk::PipelineStageFlagBits::eNone, {}, {}, nullptr, imageBarrier);
		
		
		}

		void transitionLayout(Celer::Wrapper::CommandBuffer& commandBuff, vk::raii::Image& img, vk::ImageLayout oldLayout, vk::ImageLayout newLayout) {
			vk::ImageMemoryBarrier imageBarrier{};
			vk::PipelineStageFlags src{};
			vk::PipelineStageFlags dst{};

			imageBarrier.oldLayout = oldLayout;
			imageBarrier.newLayout = newLayout;
			imageBarrier.dstQueueFamilyIndex = vk::QueueFamilyIgnored;
			imageBarrier.srcQueueFamilyIndex = vk::QueueFamilyIgnored;

			imageBarrier.image = img;

			imageBarrier.subresourceRange = { .aspectMask = vk::ImageAspectFlagBits::eColor, .levelCount = 1, .layerCount = 1 };

			if (oldLayout == vk::ImageLayout::eUndefined && newLayout == vk::ImageLayout::eTransferDstOptimal) {
				imageBarrier.srcAccessMask = {};
				imageBarrier.dstAccessMask = vk::AccessFlagBits::eTransferWrite;

				src = vk::PipelineStageFlagBits::eTopOfPipe;
				dst = vk::PipelineStageFlagBits::eTransfer;
			}
			else if (oldLayout == vk::ImageLayout::eTransferDstOptimal && newLayout == vk::ImageLayout::eShaderReadOnlyOptimal) {
				imageBarrier.srcAccessMask = vk::AccessFlagBits::eTransferWrite;
				imageBarrier.dstAccessMask = vk::AccessFlagBits::eShaderRead;

				src = vk::PipelineStageFlagBits::eTransfer;
				dst = vk::PipelineStageFlagBits::eFragmentShader;

			}
			else {
				throw std::runtime_error("Layout transition is not supported.");
			}

			commandBuff.getSingleBuffer().pipelineBarrier(src, dst, {}, {}, nullptr, imageBarrier);
		}

		FrameContext::FrameContext(Core::VulkanContext& ctx) {

			vk::SemaphoreTypeCreateInfo typeInfo = {
							.semaphoreType = vk::SemaphoreType::eTimeline,
							.initialValue = 0,
			};

			vk::SemaphoreCreateInfo timelineCreateInfo{ .pNext = &typeInfo };


			mFrameSyncObject = SemaphoreObject{ FrameContext::SemaphoreType::TIMELINE, vk::raii::Semaphore(*ctx.device, timelineCreateInfo) };


			vk::SemaphoreTypeCreateInfo typeInfo2 = { .semaphoreType = vk::SemaphoreType::eTimeline, .initialValue = 0 };

			vk::SemaphoreCreateInfo semaCreateInfo{ .pNext = &typeInfo2 };


			mUpload = vk::raii::Semaphore(*ctx.device, semaCreateInfo);
		}


		void FrameContext::addSemaphore(SemaphoreType semaphoreType, Core::VulkanContext &ctx) {
			switch (semaphoreType) {
				case FrameContext::SemaphoreType::BINARY:
					
					mSignalSemaphores.push_back(SemaphoreObject{ semaphoreType, vk::raii::Semaphore(*ctx.device, vk::SemaphoreCreateInfo{}) });


					break;
				case FrameContext::SemaphoreType::TIMELINE:
					{


						vk::SemaphoreTypeCreateInfo typeInfo = {
							.semaphoreType = vk::SemaphoreType::eTimeline,
							.initialValue = 0,
						};

						vk::SemaphoreCreateInfo timelineCreateInfo{ .pNext = &typeInfo };


						mSignalSemaphores.push_back(SemaphoreObject{ semaphoreType, vk::raii::Semaphore(*ctx.device, timelineCreateInfo) });

						mTimelineCounts.push_back(0);
					}

					break;

				default:
					break;


			}
		}



	}

}
