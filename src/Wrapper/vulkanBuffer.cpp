#include "vulkanBuffer.hpp"
#include <cassert>

namespace Celer {
	uint32_t findMemoryType(uint32_t typeFilter, vk::MemoryPropertyFlags properties, vk::raii::PhysicalDevice& physicalDevice) {
		vk::PhysicalDeviceMemoryProperties memProperties{ physicalDevice.getMemoryProperties() };

		for (uint32_t i{}; i < memProperties.memoryTypeCount; i++) {
			if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
				return i;
			}
		}


		throw std::runtime_error("Fresh outta luck pal, no memory types are available for u!");
	}

	namespace Wrapper {
		void Buffer::unmapMemory() {
			mVkDeviceMemory.unmapMemory();
		}

		void* Buffer::mapMemory(uint32_t offset, vk::MemoryMapFlags const& memoryMapFlags) {
			return mVkDeviceMemory.mapMemory(offset, mSize, memoryMapFlags);
		}

		Buffer::Buffer(vk::DeviceSize size, vk::BufferUsageFlags usage, vk::MemoryPropertyFlags props, Core::VulkanContext const& ctx) : mSize{size} {
			vk::BufferCreateInfo bufferInfo{ .size = size, .usage = usage, .sharingMode = vk::SharingMode::eExclusive };
			mVkBuffer = vk::raii::Buffer(*ctx.device, bufferInfo);

			vk::MemoryRequirements memRequirements{ mVkBuffer.getMemoryRequirements() }; /*Get mem reqs of buffer, find the appropriate memory in the device*/

			std::cout << "\n" << memRequirements.alignment << " ALIGNMENT!!!\n";


			std::cout << memRequirements.size << " SIZE!!!\n\n";

			vk::MemoryAllocateInfo allocInfo{ .allocationSize = memRequirements.size, .memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, props, *ctx.physicalDevice) /*Get the memory type that satisfies the buffer in the previous step*/ };

			mVkDeviceMemory = vk::raii::DeviceMemory(*ctx.device, allocInfo);

			mVkBuffer.bindMemory(*mVkDeviceMemory, 0);
		}

		vk::Buffer Buffer::getBuffer() const {
			return *mVkBuffer;
		}

		vk::DeviceSize Buffer::getSize() const {
			return mSize;
		}

		uint32_t Buffer::getQueueOwner() const {
			return mQueueOwner;
		}

		void Buffer::setQueueOwner(uint32_t newOwner) {
			mQueueOwner = newOwner;
		}

	}
}

