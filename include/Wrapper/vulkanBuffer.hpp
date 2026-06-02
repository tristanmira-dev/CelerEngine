#ifndef VULKANBUFFER_HPP
#define VULKANBUFFER_HPP

#include "contexts.hpp"
#include <initializer_list>
#include <iterator>

namespace Celer {
	namespace Wrapper {

		class Buffer {

			private:

				vk::DeviceSize mSize;

				vk::raii::Buffer mVkBuffer{ nullptr };
				vk::raii::DeviceMemory mVkDeviceMemory{ nullptr };

				uint32_t mQueueOwner{ static_cast<uint32_t>(~0) };

				uint32_t findMemoryType(uint32_t typeFilter, vk::MemoryPropertyFlags properties, vk::raii::PhysicalDevice const& physicalDevice) const;

			public:
				Buffer() = default;
				Buffer(vk::DeviceSize size, vk::BufferUsageFlags usage, vk::MemoryPropertyFlags props, Core::VulkanContext const &ctx);
				Buffer(Buffer&& src) noexcept;

				void* mapMemory();

				void unmapMemory();

				void operator=(Buffer&& src) noexcept;

				static Buffer createDeviceLocalBuffer(vk::DeviceSize deviceSize, Core::VulkanContext vulkanCtx, vk::BufferUsageFlags flags);

				inline vk::Buffer getBuffer() const {
					return *mVkBuffer;
				}

				inline vk::DeviceSize getSize() const {

					return mSize;
				}



		};


		/*Just a simple class for the buffer, device memory and a local buffer (CPU) to memcpy the data from*/
		template<typename T>
		class BufferStagingResource {
			private:
				uint32_t mBufferSize{};
				std::vector<T> mStagingBuffer;

			public:

				Wrapper::Buffer mBuffer;
				
				BufferStagingResource(std::initializer_list<T> vals) {
					mStagingBuffer.resize(vals.size());
					std::copy(vals.begin(), vals.end(), mStagingBuffer.begin());
				}

				void initVulkanBuffer(Core::VulkanContext vulkanCtx, vk::BufferUsageFlags bufferUsage) {
					mBufferSize = mStagingBuffer.size() * sizeof(mStagingBuffer[0]);
					mBuffer = Buffer(mBufferSize, bufferUsage, vk::MemoryPropertyFlagBits::eHostCoherent | vk::MemoryPropertyFlagBits::eHostVisible, vulkanCtx);
					
				}

				void appendToBuffer(std::initializer_list<T> vals) {
					std::copy(vals.begin(), vals.end(), std::back_inserter(mStagingBuffer));
				}

				void copyMemory() {
					mBuffer.mapMemory();
				}

				void copyBufferToLocal() {
					void* data{ mBuffer.mapMemory() };

					memcpy(data, mStagingBuffer.data(), mBufferSize);
				}

				vk::DeviceSize getSize() {
					return static_cast<vk::DeviceSize>(mBufferSize);
				}
		};

		

	}
}

#endif // VULKANBUFFER_HPP