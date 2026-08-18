#ifndef VULKANBUFFER_HPP
#define VULKANBUFFER_HPP

#include "contexts.hpp"
#include <initializer_list>
#include <iterator>
#include <stb_image.h>

namespace Celer {
	namespace Wrapper {

		class Buffer {

			private:

				vk::DeviceSize mSize{};

				vk::raii::Buffer mVkBuffer{ nullptr };
				vk::raii::DeviceMemory mVkDeviceMemory{ nullptr };

				uint32_t mQueueOwner{ static_cast<uint32_t>(~0) };

				bool mIsMapped{ false };

			public:
				//static std::pair<vk::raii::Buffer, vk::raii::DeviceMemory> createBuffer(vk::DeviceSize size, vk::BufferUsageFlags usage, vk::MemoryPropertyFlags props, Core::VulkanContext const& ctx);
				Buffer() = default;


				Buffer(vk::DeviceSize size, vk::BufferUsageFlags usage, vk::MemoryPropertyFlags props, Core::VulkanContext const &ctx);
				Buffer(Buffer&& src) noexcept;

				Buffer(Buffer const& src) = delete;

				void* mapMemory();

				void unmapMemory();


				static uint32_t findMemoryType(uint32_t typeFilter, vk::MemoryPropertyFlags properties, vk::raii::PhysicalDevice& physicalDevice);

				void operator=(Buffer&& src) noexcept;

				static Buffer createDeviceLocalBuffer(vk::DeviceSize deviceSize, Core::VulkanContext vulkanCtx, vk::BufferUsageFlags flags);

				inline vk::Buffer getBuffer() const {
					return *mVkBuffer;
				}

				inline vk::DeviceSize getSize() const {
					return mSize;
				}

				inline uint32_t getQueueOwner() {
					return mQueueOwner;
				}

				inline void setQueueOwner(uint32_t newOwner) {
					mQueueOwner = newOwner;
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

				BufferStagingResource(BufferStagingResource<T> const &stagingResource) = delete;

				BufferStagingResource(BufferStagingResource<T> &&stagingResource) noexcept {
					mBufferSize = stagingResource.mBufferSize;
					mStagingBuffer = std::move(stagingResource.mStagingBuffer);
					mBuffer = std::move(stagingResource.mBuffer);
				}

				void operator=(BufferStagingResource<T>&& stagingResource) noexcept {
					mBufferSize = stagingResource.mBufferSize;
					mStagingBuffer = std::move(stagingResource.mStagingBuffer);
					mBuffer = std::move(stagingResource.mBuffer);
				}
				
				BufferStagingResource(std::initializer_list<T> vals) {
					mStagingBuffer.resize(vals.size());
					std::copy(vals.begin(), vals.end(), mStagingBuffer.begin());
				}

				BufferStagingResource(void* data, uint64_t size, Core::VulkanContext& vulkanCtx) {
					mStagingBuffer.resize(size / sizeof(T));

					memcpy(mStagingBuffer.data(), data, size);

					mBufferSize = mStagingBuffer.size() * sizeof(T);
					mBuffer = Buffer(mBufferSize, vk::BufferUsageFlagBits::eTransferSrc, vk::MemoryPropertyFlagBits::eHostCoherent | vk::MemoryPropertyFlagBits::eHostVisible, vulkanCtx);
					
					
					copyBufferToLocal();
				}

				void initVulkanBuffer(Core::VulkanContext vulkanCtx, vk::BufferUsageFlags bufferUsage) {
					mBufferSize = mStagingBuffer.size() * sizeof(mStagingBuffer[0]);
					mBuffer = Buffer(mBufferSize, bufferUsage, vk::MemoryPropertyFlagBits::eHostCoherent | vk::MemoryPropertyFlagBits::eHostVisible, vulkanCtx);
					
				}

				void appendToBuffer(std::initializer_list<T> vals) {
					std::copy(vals.begin(), vals.end(), std::back_inserter(mStagingBuffer));
				}

				void copyBufferToLocal() {
					void* data{ mBuffer.mapMemory() };

					memcpy(data, mStagingBuffer.data(), mBufferSize);
				}

				vk::DeviceSize getSize() {
					return static_cast<vk::DeviceSize>(mBufferSize);
				}

				vk::Buffer getUnderlyingBuffer() {
					return mBuffer.getBuffer();
				}

				~BufferStagingResource() {
					mBuffer.unmapMemory();
				}
		};

		

	}
}

#endif // VULKANBUFFER_HPP