#ifndef WRAPPER_HOST_VISIBLE_BUFFER_HPP
#define WRAPPER_HOST_VISIBLE_BUFFER_HPP

#include "vulkanBuffer.hpp"

namespace Celer {

	namespace Wrapper {

		class HostBuffer : public Buffer {
			private:
				void* mMappedMemory{ nullptr };
			public:

				HostBuffer(HostBuffer&& src) noexcept;
				HostBuffer& operator=(HostBuffer&& src) noexcept;

				HostBuffer(HostBuffer const& src) = delete;
				HostBuffer& operator=(HostBuffer const& src) = delete;

				~HostBuffer();

				HostBuffer(vk::DeviceSize size, vk::BufferUsageFlags usage, Core::VulkanContext const& ctx);
				HostBuffer(vk::DeviceSize size, Core::VulkanContext const& ctx);
				void* getMappedMemory();

		};


	}


}





#endif // WRAPPER_HOST_VISIBLE_BUFFER_HPP
