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
				~Image() = default;
		};

		template<typename ImageType>
		class ImageCollection {

			private:
				std::vector<Image<ImageType>> mImageCollection;
				
			public:

				void clear();
				~ImageCollection();
		};


		using OwnedImageCollection = ImageCollection<vk::raii::Image>;
		using NonOwnedImageCollection = ImageCollection<vk::Image>;



	}
}

#endif // IMAGE_HPP