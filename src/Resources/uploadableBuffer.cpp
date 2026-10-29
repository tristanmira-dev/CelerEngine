#include "uploadableBuffer.hpp"

namespace Celer {

	namespace Resource {
		BufferTransfer::BufferTransfer(vk::Buffer buffer, uint32_t size, uint32_t offset) : mBufferHandle{ buffer }, mSize{ size }, mOffset{ offset } {
		}

		void BufferTransfer::acquire(Wrapper::CommandBuffer& commandBuff, Core::VulkanContext& ctx) {

			vk::BufferMemoryBarrier acquire{
				.srcAccessMask = vk::AccessFlagBits::eNone,
				.dstAccessMask = vk::AccessFlagBits::eVertexAttributeRead,
				.srcQueueFamilyIndex = ctx.transferQueueIdx,
				.dstQueueFamilyIndex = ctx.graphicsQueueIdx,
				.buffer = mBufferHandle,
				.offset = mOffset,
				.size = mSize
			};

			commandBuff.getSingleBuffer().pipelineBarrier(vk::PipelineStageFlagBits::eTopOfPipe, vk::PipelineStageFlagBits::eVertexInput, {}, {}, acquire, {});

		}

		UploadableBuffer::UploadableBuffer(Core::VulkanContext& ctx, vk::Buffer buffer, Core::Memory memory, void* data, uint32_t sizeInBytes) : mBufferHandle{ buffer }, mMemory{ memory }, mStagingBuffer(sizeInBytes, ctx) {
			
			mStagingBuffer.addToLocalContainer(data);
			mStagingBuffer.copyToMappedBuffer();

		}

		std::unique_ptr<OwnershipTransfer> UploadableBuffer::upload(Wrapper::CommandBuffer& commandBuff, Core::VulkanContext& ctx)
		{
			commandBuff.getSingleBuffer().copyBuffer(mStagingBuffer.getUnderlyingBuffer(), mBufferHandle, vk::BufferCopy(0, mMemory.getOffset(), mStagingBuffer.getSizeInBytes()));

			/*buddy, you already know this is bad, plz put this in ownership transfer*/
			vk::BufferMemoryBarrier release{
				.srcAccessMask = vk::AccessFlagBits::eTransferWrite,
				.dstAccessMask = vk::AccessFlagBits::eNone,
				.srcQueueFamilyIndex = ctx.transferQueueIdx,
				.dstQueueFamilyIndex = ctx.graphicsQueueIdx,
				.buffer = mBufferHandle,
				.offset = mMemory.getOffset(),
				.size = mMemory.getSize(),
			};

			commandBuff.getSingleBuffer().pipelineBarrier(vk::PipelineStageFlagBits::eTransfer, vk::PipelineStageFlagBits::eBottomOfPipe, {}, {}, release, {});

			return std::make_unique<BufferTransfer>(mBufferHandle, mMemory.getSize(), mMemory.getOffset());
		}

		std::unique_ptr<UploadableBuffer> UploadableBuffer::create(Core::VulkanContext& ctx, vk::Buffer buffer, Core::Memory memory, void* data, uint32_t sizeInBytes)
		{
			return std::make_unique<UploadableBuffer>(ctx, buffer, memory, data, sizeInBytes);
		}
	}

}