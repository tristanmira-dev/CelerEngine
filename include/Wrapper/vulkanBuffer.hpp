#ifndef VULKANBUFFER_HPP
#define VULKANBUFFER_HPP

#include "contexts.hpp"

namespace Celer {
	namespace Wrapper {

		class Buffer {

			private:

				vk::DeviceSize mSize;

				vk::raii::Buffer mVkBuffer{ nullptr };
				vk::raii::DeviceMemory mVkDeviceMemory{ nullptr };

				uint32_t queueOwner{ static_cast<uint32_t>(~0) };

				uint32_t findMemoryType(uint32_t typeFilter, vk::MemoryPropertyFlags properties, vk::raii::PhysicalDevice const& physicalDevice) const;
			public:
				Buffer() = default;
				Buffer(vk::DeviceSize size, vk::BufferUsageFlags usage, vk::MemoryPropertyFlags props, Core::VulkanContext const &ctx);

				inline vk::Buffer getBuffer() {
					return *mVkBuffer;
				}



		};


		/*Just a simple class for the buffer, device memory and a local buffer (CPU) to memcpy the data from*/
		template<typename T>
		class BufferResource {
			private:
				uint32_t mBufferSize{};
				Wrapper::Buffer mBuffer;
				std::vector<T> mStagingBuffer;
			public:

				
				BufferResource(Core::VulkanContext vulkanCtx, uint32_t memSize) : mBufferSize{ memSize }, mBuffer(memSize,
					vk::BufferUsageFlagBits::eVertexBuffer | vk::BufferUsageFlagBits::eTransferDst, vk::MemoryPropertyFlagBits::eDeviceLocal, vulkanCtx) {

				}
		};

		

	}
}

#endif // VULKANBUFFER_HPP