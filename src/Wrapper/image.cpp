#include "image.hpp"

namespace {
	template<typename ImageType>
	vk::Image const& getUnderlyingImage(ImageType& image) {
		return image;
	}

	template<>
	vk::Image const& getUnderlyingImage<vk::raii::Image>(vk::raii::Image& image) {
		return *image;
	}
}

namespace Celer {


	namespace Wrapper {

	
		/*Image DEFINITIONS!*/

		template<typename ImageType>
		Image<ImageType>::Image(ImageType &&image, vk::raii::ImageView &&imageView) : mImage{ std::move(image) }, mImageView{ std::move(imageView) } { }

		template<typename ImageType>
		Image<ImageType>::Image(Image<ImageType>&& image) noexcept : mImage{ std::move(image.mImage) }, mImageView{std::move(image.mImageView)} { }



		/*ImageCollection DEFINITIONS!*/


		template<typename ImageType>
		void ImageCollection<ImageType>::clear() {
			mImageCollection.clear();
		}

		template<typename ImageType>
		void ImageCollection<ImageType>::addImagesWithView(std::vector<ImageType> images, vk::Format format, vk::raii::Device &device) {
		
			vk::ImageViewCreateInfo imageViewCreateInfo{
				.viewType = vk::ImageViewType::e2D,
				.format = format,
				.subresourceRange = { vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1 },
			};

			std::for_each(images.begin(), images.end(), [this, &imageViewCreateInfo, &device](ImageType &image) {
				imageViewCreateInfo.image = getUnderlyingImage(image);
				this->mImageCollection.emplace_back( std::move(image), vk::raii::ImageView(device, imageViewCreateInfo) );
			});
		
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

