#ifndef RESOURCES_UPLOADABLE_BUFFER_HPP
#define RESOURCES_UPLOADABLE_BUFFER_HPP

#include "uploadableResource.hpp"
#include "deviceMemManager.hpp"
#include "ownershipTransfer.hpp"
#include "stagingBuffer.hpp"

namespace Celer {

	namespace Resource {

		class BufferTransfer : public OwnershipTransfer {
			private:
				vk::Buffer mBufferHandle;
				uint32_t mSize;
				uint32_t mOffset;
			public:
				BufferTransfer(vk::Buffer buffer, uint32_t size, uint32_t offset);
				void acquire(Wrapper::CommandBuffer& commandBuff, Core::VulkanContext& ctx);
				static std::unique_ptr<BufferTransfer> create(vk::Buffer buffer, uint32_t size, uint32_t offset);
		};



		class UploadableBuffer : public UploadableResource {
			private:
				vk::Buffer mBufferHandle;
				Core::Memory mMemory{};
				Wrapper::StagingBuffer<std::uint8_t> mStagingBuffer;

			public:
				UploadableBuffer(Core::VulkanContext& ctx, vk::Buffer buffer, Core::Memory memory, void* data, uint32_t sizeInBytes);
				~UploadableBuffer() = default;
				virtual std::unique_ptr<OwnershipTransfer> upload(Wrapper::CommandBuffer& commandBuff, Core::VulkanContext& ctx) override;
				static std::unique_ptr<UploadableBuffer> create(Core::VulkanContext& ctx, vk::Buffer buffer, Core::Memory memory, void* data, uint32_t sizeInBytes);

		};

	}


}








#endif // RESOURCES_UPLOADABLE_BUFFER_HPP