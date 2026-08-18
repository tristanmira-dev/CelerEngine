#include "commandBuffer.hpp"

namespace Celer {

	namespace Wrapper {
		void CommandBuffer::resetSingleBuff() {

			mCommandBuffers[0].reset();

		}
		CommandBuffer::CommandBuffer(vk::raii::Device &device, uint32_t count, uint32_t queueFamilyIdx, vk::CommandBufferLevel bufferLevel) {
			
			vk::CommandPoolCreateInfo poolInfo{.flags = vk::CommandPoolCreateFlagBits::eResetCommandBuffer, .queueFamilyIndex = queueFamilyIdx};

			mCommandPool = vk::raii::CommandPool(device, poolInfo);


			vk::CommandBufferAllocateInfo commandBuffersInfo{ .commandPool = mCommandPool, .level = bufferLevel, .commandBufferCount = count };

			mCommandBuffers = vk::raii::CommandBuffers(device, commandBuffersInfo);
			
		}

		void CommandBuffer::beginSingleTimeCommand() {
			mCommandBuffers[0].begin({ .flags = vk::CommandBufferUsageFlagBits::eOneTimeSubmit });
		}

		vk::raii::CommandBuffer& CommandBuffer::getSingleBuffer() {
			return *mCommandBuffers.begin();
		}

		void CommandBuffer::endSyncCommand(vk::raii::Queue& queue, uint32_t commandBufferIdx, vk::raii::Semaphore* signalSemaphore, vk::raii::Semaphore* waitSemaphore, vk::PipelineStageFlags pipelineStage, vk::raii::Fence *fence) {

			mCommandBuffers[commandBufferIdx].end();

			vk::SubmitInfo submit{ .commandBufferCount = 1, .pCommandBuffers = &*mCommandBuffers[commandBufferIdx] };

			if (signalSemaphore) {
				submit.pSignalSemaphores = &(**signalSemaphore);
				submit.signalSemaphoreCount = 1;
			}

			if (waitSemaphore) {
				submit.pWaitSemaphores = &(**waitSemaphore);
				submit.waitSemaphoreCount = 1;
				submit.pWaitDstStageMask = &pipelineStage;
			}

			if (fence) queue.submit(submit, **fence);
			else queue.submit(submit);
		

		}

		void CommandBuffer::endSingleTimeCommand(vk::raii::Queue& queue, vk::raii::Semaphore* semaphore) {
			mCommandBuffers[0].end();

			vk::SubmitInfo submitInfo{ .commandBufferCount = 1, .pCommandBuffers = &**mCommandBuffers.begin() /*holy this monstrosity*/ };

			queue.submit(submitInfo);

			//mCommandBuffers[0].reset();
		}

		



	}

}