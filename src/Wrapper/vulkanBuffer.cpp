#include "vulkanBuffer.hpp"


namespace Celer {
	namespace Wrapper {
		uint32_t Buffer::findMemoryType(uint32_t typeFilter, vk::MemoryPropertyFlags properties, vk::raii::PhysicalDevice const& physicalDevice) const {
			vk::PhysicalDeviceMemoryProperties memProperties{ physicalDevice.getMemoryProperties() };

			for (uint32_t i{}; i < memProperties.memoryTypeCount; i++) {
				if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
					return i;
				}
			}


			throw std::runtime_error("Fresh outta luck pal, no memory types are available for u!");
		}
		Buffer::Buffer(vk::DeviceSize size, vk::BufferUsageFlags usage, vk::MemoryPropertyFlags props, Core::VulkanContext const& ctx) {
			vk::BufferCreateInfo bufferInfo{ .size = size, .usage = usage, .sharingMode = vk::SharingMode::eExclusive };
			mVkBuffer = vk::raii::Buffer(*ctx.device, bufferInfo);

			vk::MemoryRequirements memRequirements{ mVkBuffer.getMemoryRequirements() }; /*Get mem reqs of buffer, find the appropriate memory in the device*/
			vk::MemoryAllocateInfo allocInfo{ .allocationSize = memRequirements.size, .memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, props, *ctx.physicalDevice) /*Get the memory type that satisfies the buffer in the previous step*/ };

			mVkDeviceMemory = vk::raii::DeviceMemory(*ctx.device, allocInfo);

			mVkBuffer.bindMemory(*mVkDeviceMemory, 0);
		}

	}
}

