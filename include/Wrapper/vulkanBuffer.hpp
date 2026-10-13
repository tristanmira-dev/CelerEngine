#ifndef VULKANBUFFER_HPP
#define VULKANBUFFER_HPP

#include "contexts.hpp"
#include <initializer_list>
#include <iterator>
#include <stb_image.h>

namespace Celer {

	uint32_t findMemoryType(uint32_t typeFilter, vk::MemoryPropertyFlags properties, vk::raii::PhysicalDevice& physicalDevice);

	namespace Wrapper {


		//set up buffer type
		//getters for the handles
		//don't need any staging resources!!!
		//just a simple wrapper to facilitate seamless creation of vkBuffers
		class Buffer {

			private:

				vk::DeviceSize mSize{};

				vk::raii::Buffer mVkBuffer{ nullptr };
				vk::raii::DeviceMemory mVkDeviceMemory{ nullptr };

				uint32_t mQueueOwner{ static_cast<uint32_t>(~0) }; //Might just remove, or keep for tracking purposes

			protected:
				void* mapMemory(uint32_t offset, vk::MemoryMapFlags const& memoryMapFlags);
				void unmapMemory();

			public:
				Buffer() = default;
				Buffer(Buffer&& src) = default;
				Buffer& operator=(Buffer&& src) = default;
				Buffer(vk::DeviceSize size, vk::BufferUsageFlags usage, vk::MemoryPropertyFlags props, Core::VulkanContext const &ctx);
				~Buffer() = default;
				

				Buffer(Buffer const& src) = delete;
				Buffer& operator=(Buffer const& src) = delete;

				vk::Buffer getBuffer() const;

				vk::DeviceSize getSize() const;

				uint32_t getQueueOwner() const;

				void setQueueOwner(uint32_t newOwner);

		};

		/*TODO!!!!!!!!!!!!!!!!!!!!------------------------------------------------------*/
		template<uint32_t size>
		class StaticStagingResource {
			private:
				std::array<uint8_t, size> mBufferData;
			public:
				Wrapper::Buffer mStagingBuffer;
				


		};


		

	}
}

#endif // VULKANBUFFER_HPP