#include "image.hpp"


namespace {
	template<typename ImageType>
	inline vk::Image const& getUnderlyingImage(ImageType& image) {
		return image;
	}

	template<>
	inline vk::Image const& getUnderlyingImage<vk::raii::Image>(vk::raii::Image& image) {
		return *image;
	}
}

namespace Celer {


	namespace Wrapper {

	
		/*Image DEFINITIONS!*/

		template<typename ImageType>
		Image<ImageType>::Image(ImageType &&image, vk::raii::ImageView &&imageView) : mImage{ std::move(image) }, mImageView{ std::move(imageView) } { }


		template<typename ImageType>
		ImageType& Image<ImageType>::imageRef() {
			return mImage;
		}

		template<typename ImageType>
		Image<ImageType>::Image(Image<ImageType>&& image) noexcept : mImage{ std::move(image.mImage) }, mImageView{ std::move(image.mImageView) } {}

		template<>
		void Image<vk::raii::Image>::setImageView(vk::raii::Device& device, vk::Format format) {
			vk::ImageViewCreateInfo imageViewCreateInfo{
				.image = mImage,
				.viewType = vk::ImageViewType::e2D,
				.format = format,
				.subresourceRange = {.aspectMask = vk::ImageAspectFlagBits::eColor, .baseMipLevel = 0, .levelCount = 1, .baseArrayLayer = 0, .layerCount = 1} 
			};


			imageViewCreateInfo.image = *mImage;	

			mImageView = vk::raii::ImageView(device, imageViewCreateInfo);
		}

		template<>
		vk::MemoryRequirements Image<vk::raii::Image>::getImageMemoryReq() {
			return mImage.getMemoryRequirements();
		}

		template<>
		vk::MemoryRequirements Image<vk::Image>::getImageMemoryReq() {
			return vk::MemoryRequirements{};
		}

		template<typename ImageType>
		void Image<ImageType>::transitionImageLayout(vk::ImageLayout oldLayout, vk::ImageLayout newLayout, vk::AccessFlags2 srcAccessMask, vk::AccessFlags2 dstAccessMask, vk::PipelineStageFlags2 srcStageMask, vk::PipelineStageFlags2 dstStageMask, vk::ImageAspectFlags aspectFlag, vk::raii::CommandBuffer& commandBuff) {
			vk::ImageMemoryBarrier2 imageTransitionAndWait {
				.srcStageMask = srcStageMask,
				.srcAccessMask = srcAccessMask,
				.dstStageMask = dstStageMask,
				.dstAccessMask = dstAccessMask,
				.oldLayout = oldLayout,
				.newLayout = newLayout,
				.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
				.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
				.image = getUnderlyingImage(mImage),
				.subresourceRange = {
					.aspectMask = aspectFlag,
					.baseMipLevel = 0,
					.levelCount = 1,
					.baseArrayLayer = 0,
					.layerCount = 1
				}
			};


			vk::DependencyInfo dependencyInfo{
				.dependencyFlags = {},
				.imageMemoryBarrierCount = 1,
				.pImageMemoryBarriers = &imageTransitionAndWait /* "everything before this barrier must finish before anything after it can start.", The layout transition is the barrier */
			};

			commandBuff.pipelineBarrier2(dependencyInfo);
		}

		/*ImageCollection DEFINITIONS!*/


		template<typename ImageType>
		void ImageCollection<ImageType>::clear() {
			mImageCollection.clear();
		}

		template<typename ImageType>
		void ImageCollection<ImageType>::addImagesWithView(std::vector<ImageType> &&images, vk::Format format, vk::raii::Device &device) {
		
			vk::ImageViewCreateInfo imageViewCreateInfo{
				.viewType = vk::ImageViewType::e2D,
				.format = format,
				.subresourceRange = { vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1 },
			};

			std::for_each(images.begin(), images.end(), [this, &imageViewCreateInfo, &device](ImageType &image) {
				imageViewCreateInfo.image = getUnderlyingImage(image);
				this->mImageCollection.emplace_back( std::move(image), vk::raii::ImageView(device, imageViewCreateInfo) ); //Check exception safety for this
				++mSize;
			});
		
		}

		template<typename ImageType>
		void ImageCollection<ImageType>::addOwnedImage(vk::Format format, vk::raii::Device& device, vk::Extent3D extents, vk::ImageTiling tiling, vk::ImageUsageFlags usage) {
			vk::ImageCreateInfo imageCreateInfo{
				.imageType = vk::ImageType::e2D,
				.format = format,
				.extent = extents,
				.mipLevels = 1,
				.arrayLayers = 1,
				.samples = vk::SampleCountFlagBits::e1,
				.tiling = tiling,
				.usage = usage,
				.sharingMode = vk::SharingMode::eExclusive 
			};

			vk::raii::Image image(device, imageCreateInfo);
			

			//vk::ImageViewCreateInfo imageViewCreateInfo{
			//	.image = image,
			//	.viewType = vk::ImageViewType::e2D,
			//	.format = format,
			//	.subresourceRange = {.aspectMask = vk::ImageAspectFlagBits::eColor, .baseMipLevel = 0, .levelCount = 1, .baseArrayLayer = 0, .layerCount = 1} 
			//};

			//vk::raii::ImageView imageView{ vk::raii::ImageView(device, imageViewCreateInfo) };
			
			//imageViewCreateInfo.image = *image;
			mImageCollection.emplace_back(std::move(image), nullptr);
		
		}

		template<typename ImageType>
		ImageCollection<ImageType>::~ImageCollection() {
			clear();
		}

		/*FORCE INSTANTIATE THESE TEMPLATE CLASSES WITH THESE TYPES*/

		template class Image<vk::raii::Image>;
		template class Image<vk::Image>;


		template class ImageCollection<vk::raii::Image>;
		template class ImageCollection<vk::Image>;


	}

}

