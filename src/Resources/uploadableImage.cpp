#include "uploadableImage.hpp"
#include "sync.hpp"

namespace Celer {

	namespace Resource {

		UploadableImage::UploadableImage(uint32_t width, uint32_t height, void* data, Core::VulkanContext& ctx, vk::Image image, Core::Memory memory) : UploadableResource(), mWidth{ width }, mHeight{ height }, mImageHandle{ image }, mMemory{ memory }, mBuffer(memory.getSize(), ctx) {
			mBuffer.addToLocalContainer(data);
			mBuffer.copyToMappedBuffer();
		}

		std::unique_ptr<OwnershipTransfer> UploadableImage::upload(Wrapper::CommandBuffer& commandBuff, Core::VulkanContext& ctx) {

			Core::transitionLayout(commandBuff, mImageHandle, vk::ImageLayout::eUndefined, vk::ImageLayout::eTransferDstOptimal);

			vk::BufferImageCopy region{ .bufferOffset = 0,
											   .bufferRowLength = 0,
											   .bufferImageHeight = 0,
											   .imageSubresource = {.aspectMask = vk::ImageAspectFlagBits::eColor, .mipLevel = 0, .baseArrayLayer = 0, .layerCount = 1},
											   .imageOffset = {0, 0, 0},
											   .imageExtent = {mWidth, mHeight, 1
			} };

			commandBuff.getSingleBuffer().copyBufferToImage(mBuffer.getUnderlyingBuffer(), mImageHandle, vk::ImageLayout::eTransferDstOptimal, region);

			Core::releaseBarrier(commandBuff, mImageHandle, ctx.transferQueueIdx, ctx.graphicsQueueIdx); //bad, separate it out todo plz

			return ImageTransfer::create(mImageHandle);
	

		}

		std::unique_ptr<UploadableImage> UploadableImage::create(uint32_t width, uint32_t height, void* data, Core::VulkanContext& ctx, vk::Image image, Core::Memory memory) {
			return std::make_unique<UploadableImage>(width, height, data, ctx, image, memory);
		}

		/*----------------------------------------------------------Image Transfer--------------------------------------------------------------*/
		ImageTransfer::ImageTransfer(vk::Image image) : mImageHandle{ image } {}

		void ImageTransfer::acquire(Wrapper::CommandBuffer& commandBuff, Core::VulkanContext& ctx) {

			vk::ImageMemoryBarrier imgMemory{
				.srcAccessMask = vk::AccessFlagBits::eNone,
				.dstAccessMask = vk::AccessFlagBits::eShaderRead,
				.oldLayout = vk::ImageLayout::eTransferDstOptimal,
				.newLayout = vk::ImageLayout::eShaderReadOnlyOptimal,
				.srcQueueFamilyIndex = ctx.transferQueueIdx,
				.dstQueueFamilyIndex = ctx.graphicsQueueIdx,
				.image = mImageHandle,
			};

			imgMemory.subresourceRange = { .aspectMask = vk::ImageAspectFlagBits::eColor, .levelCount = 1, .layerCount = 1 };

			commandBuff.getSingleBuffer().pipelineBarrier(vk::PipelineStageFlagBits::eTopOfPipe, vk::PipelineStageFlagBits::eFragmentShader, {}, {}, {}, imgMemory);
		
		}

		std::unique_ptr<ImageTransfer> ImageTransfer::create(vk::Image image) {
			return std::make_unique<ImageTransfer>(image);
		}



	}

}