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

		void transitionImageLayout(vk::Image image, vk::ImageLayout oldLayout, vk::ImageLayout newLayout, vk::AccessFlags2 srcAccessMask, vk::AccessFlags2 dstAccessMask, vk::PipelineStageFlags2 srcStageMask, vk::PipelineStageFlags2 dstStageMask, vk::ImageAspectFlags aspectFlag, vk::raii::CommandBuffer& commandBuff) {
			vk::ImageMemoryBarrier2 imageTransitionAndWait{
				.srcStageMask = srcStageMask,
				.srcAccessMask = srcAccessMask,
				.dstStageMask = dstStageMask,
				.dstAccessMask = dstAccessMask,
				.oldLayout = oldLayout,
				.newLayout = newLayout,
				.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
				.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
				.image = image,
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
				.pImageMemoryBarriers = &imageTransitionAndWait
			};

			commandBuff.pipelineBarrier2(dependencyInfo);
		}


		ImageCollection::ImageCollection(ImageCollection&& src) noexcept : mImageCollection{ std::move(src.mImageCollection) } { /*EMPTY BY DESIGN*/ }

		ImageCollection& ImageCollection::operator=(ImageCollection&& src) noexcept {

			if (this == &src) return *this; //Check if the moving the same object

			mImageCollection = std::move(src.mImageCollection);

			return *this;
		}

		RaiiImage& ImageCollection::operator[](std::size_t idx) {
			return mImageCollection[idx];
		}

		std::size_t ImageCollection::size() {
			return mImageCollection.size();
		}

		RaiiImage& ImageCollection::back() {
			return mImageCollection.back();
		}

		void ImageCollection::addImage(vk::Format format, vk::raii::Device& device, vk::Extent3D extents, vk::ImageTiling tiling, vk::ImageUsageFlags usage) {
			
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

			mImageCollection.emplace_back(std::move(image), nullptr);

		}

		RaiiImage::RaiiImage(RaiiImage&& image) noexcept : mImage{ std::move(image.mImage) }, mImageView{ std::move(image.mImageView) } { /*EMPTY BY DESIGN*/ }

		RaiiImage& RaiiImage::operator=(RaiiImage&& image) noexcept {

			if (this == &image) return *this; //Check if the moving the same object

			mImage = std::move(image.mImage);
			mImageView = std::move(image.mImageView);

			return *this;

		}

		RaiiImage::RaiiImage(vk::raii::Image&& image, vk::raii::ImageView&& imageView) noexcept : mImage{ std::move(image) }, mImageView{ std::move(imageView) } { /*EMPTY BY DESIGN*/ }

		RaiiImage::RaiiImage(vk::raii::Image&& image) noexcept : mImage{ std::move(image) } { /*EMPTY BY DESIGN*/ }

		void RaiiImage::setImageView(vk::raii::Device& device, vk::Format format) {
			vk::ImageViewCreateInfo imageViewCreateInfo{
				.image = *mImage,
				.viewType = vk::ImageViewType::e2D,
				.format = format,
				.subresourceRange = {.aspectMask = vk::ImageAspectFlagBits::eColor, .baseMipLevel = 0, .levelCount = 1, .baseArrayLayer = 0, .layerCount = 1}
			};

			mImageView = vk::raii::ImageView(device, imageViewCreateInfo);
		}

		void RaiiImage::setImageView(vk::raii::Device& device, vk::ImageViewCreateInfo&& createInfo) {

			createInfo.image = *mImage;

			mImageView = vk::raii::ImageView(device, createInfo);

		}

		vk::raii::Image const& RaiiImage::getImage() {
			return mImage;
		}

		vk::raii::ImageView const& RaiiImage::getImageView() {
			return mImageView;
		}

		vk::MemoryRequirements RaiiImage::getImageMemoryReq() {
			return mImage.getMemoryRequirements();
		}



	}

}

