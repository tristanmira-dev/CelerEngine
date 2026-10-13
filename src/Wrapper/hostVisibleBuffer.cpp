#include "hostVisibleBuffer.hpp"

namespace Celer {

	namespace Wrapper {

		HostBuffer::HostBuffer(HostBuffer&& src)  noexcept : Buffer(std::move(src)) {
			std::swap(src.mMappedMemory, mMappedMemory);
		}

		HostBuffer& HostBuffer::operator=(HostBuffer&& src) noexcept {

			if (this != &src) {
				HostBuffer copy(std::move(src));

				Buffer::operator=(std::move(copy));
				std::swap(copy.mMappedMemory, mMappedMemory);
			}


			return *this;
		}

		HostBuffer::HostBuffer(vk::DeviceSize size, vk::BufferUsageFlags usage, Core::VulkanContext const& ctx)
			: Buffer(size, usage, vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent, ctx) {

			mMappedMemory = mapMemory(0, {});

		}

		HostBuffer::HostBuffer(vk::DeviceSize size, Core::VulkanContext const& ctx)
			: Buffer(size, vk::BufferUsageFlagBits::eTransferSrc, vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent, ctx) {

			mMappedMemory = mapMemory(0, {});

		}

		void* HostBuffer::getMappedMemory() {
			assert(mMappedMemory && "Warning, mapped memory is null!");
			return mMappedMemory;
		}

		HostBuffer::~HostBuffer() {
			if(mMappedMemory) unmapMemory();
		}	


	} // namespace Wrapper

} // namespace Celer