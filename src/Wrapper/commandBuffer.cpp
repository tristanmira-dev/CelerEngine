#include "commandBuffer.hpp"

namespace Celer {

	namespace Wrapper {
		CommandBuffer::CommandBuffer(vk::raii::Device &device, uint32_t count, uint32_t queueFamilyIdx, vk::CommandBufferLevel bufferLevel) {
			
			vk::CommandPoolCreateInfo poolInfo{.flags = vk::CommandPoolCreateFlagBits::eResetCommandBuffer, .queueFamilyIndex = queueFamilyIdx};

			mCommandPool = vk::raii::CommandPool(device, poolInfo);


			vk::CommandBufferAllocateInfo commandBuffersInfo{ .commandPool = mCommandPool, .level = bufferLevel, .commandBufferCount = count };

			mCommandBuffers = vk::raii::CommandBuffers(device, commandBuffersInfo);
			
		}

		



	}

}