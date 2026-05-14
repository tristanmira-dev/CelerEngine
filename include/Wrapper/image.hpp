#ifndef IMAGE_HPP
#define IMAGE_HPP

namespace Celer {

	namespace Wrapper {

		template<typename ImageType>
		class Image {
			private:
				ImageType mImage;
			public:
				vk::raii::ImageView mImageView{ nullptr };
				
				Image(ImageType &&image, vk::raii::ImageView &&imageView);

				Image(Image&& image) noexcept;

				~Image() = default;
		};

		template<typename ImageType>
		class ImageCollection {

			private:
				std::vector<Image<ImageType>> mImageCollection;
				
			public:

				void clear();
				void addImagesWithView(std::vector<ImageType> images, vk::Format format, vk::raii::Device& device);
				//void addImages(std::vector<ImageType> const &images);

				~ImageCollection();
		};

		/*Tell the compiler when they see these types, don't generate the implementation*/

		extern template class Image<vk::raii::Image>;
		extern template class Image<vk::Image>;

		extern template class ImageCollection<vk::raii::Image>;
		extern template class ImageCollection<vk::Image>;

		/*Typedefs*/

		using OwnedImageCollection = ImageCollection<vk::raii::Image>;
		using NonOwnedImageCollection = ImageCollection<vk::Image>;

		using OwnedImage = Image<vk::raii::Image>;
		using NonOwnedImage = Image<vk::Image>;





	}
}

#endif // IMAGE_HPP