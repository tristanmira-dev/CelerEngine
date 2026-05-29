#ifndef VULKANBUFFER_HPP
#define VULKANBUFFER_HPP

#include "contexts.hpp"

namespace Celer {
	namespace Wrapper {

		class Buffer {

			private:

				vk::raii::Buffer mVkBuffer{ nullptr };
				vk::raii::DeviceMemory mVkDeviceMemory{ nullptr };

				uint32_t queueOwner{ static_cast<uint32_t>(~0) };

				uint32_t findMemoryType(uint32_t typeFilter, vk::MemoryPropertyFlags properties, vk::raii::PhysicalDevice const& physicalDevice) const;
			public:
				Buffer() = default;
				Buffer(vk::DeviceSize size, vk::BufferUsageFlags usage, vk::MemoryPropertyFlags props, Core::VulkanContext const &ctx);

		};

	}
}

#endif // VULKANBUFFER_HPP