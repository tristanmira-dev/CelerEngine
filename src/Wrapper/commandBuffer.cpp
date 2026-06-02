#include "commandBuffer.hpp"

namespace Celer {

	namespace Wrapper {
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

		void CommandBuffer::endSingleTimeCommand(vk::raii::Queue& queue) {
			mCommandBuffers[0].end();

			queue.submit(vk::SubmitInfo{ .commandBufferCount = 1, .pCommandBuffers = &**mCommandBuffers.begin() /*holy this monstrosity*/ });

			queue.waitIdle();
			mCommandBuffers[0].reset();
		}

		



	}

}