#ifndef IMAGE_HPP
#define IMAGE_HPP

namespace Celer {

	namespace Wrapper {

		//const vk::Format TEXTURE_FORMAT{};

		class RaiiImage {
			private:
				vk::raii::Image mImage{ nullptr };
				vk::raii::ImageView mImageView{ nullptr };
			public:
				/*DELETED FUNCTS*/
				RaiiImage(RaiiImage const& image) = delete;
				RaiiImage& operator=(RaiiImage const& image) = delete;
				RaiiImage(vk::raii::Image const& image, vk::raii::ImageView const& imageView) = delete;
				//

				RaiiImage() = default;
				~RaiiImage() = default;

				/*MOVE ASSIGNMENT, MOVE CONSTRUCTOR*/
				RaiiImage(RaiiImage&& image) noexcept;
				RaiiImage& operator=(RaiiImage&& image) noexcept;
				//

				/*MOVES*/
				RaiiImage(vk::raii::Image&& image, vk::raii::ImageView&& imageView) noexcept;
				RaiiImage(vk::raii::Image&& image) noexcept;
				//

				//Setters
				
				//Default for texture image views
				void setImageView(vk::raii::Device& device, vk::Format format);

				//More specific create info, image is set automatically to the member image
				void setImageView(vk::raii::Device& device, vk::ImageViewCreateInfo&& createInfo);

				//
				
				//Getters
				vk::raii::Image const& getImage();
				vk::raii::ImageView const& getImageView();

				//Helpers
				vk::MemoryRequirements getImageMemoryReq();


		};

		class ImageCollectionRefactor {

			private:

				std::vector<RaiiImage> mImageCollection;

			public:

				
				ImageCollectionRefactor() = default;
				~ImageCollectionRefactor() = default;

				//DELETE THESE COPY ASSIGNMENT AND COPY CONSTRUCTORS CUZ OF RAII ELEMENTS OF STD VECTOR
				ImageCollectionRefactor(ImageCollectionRefactor const& src) = delete;
				ImageCollectionRefactor& operator=(ImageCollectionRefactor const& src) = delete;
				//

				//MOVE CONSTRUCTOR AND MOVE ASSIGNMENT
				ImageCollectionRefactor(ImageCollectionRefactor &&src) noexcept;
				ImageCollectionRefactor& operator=(ImageCollectionRefactor&& src) noexcept;
				//		

				//OVERLOADS
				RaiiImage& operator[](std::size_t idx);
				//

				//UTILS
				std::size_t size();
				RaiiImage& back();

				//Add default texture images
				void addImage(vk::Format format, vk::raii::Device& device, vk::Extent3D extents, vk::ImageTiling tiling, vk::ImageUsageFlags usage);


		};



		template<typename ImageType>
		class Image {
			private:
				ImageType mImage;
			public:
				vk::raii::ImageView mImageView{ nullptr };

				inline ImageType& getImage() {
					return mImage;
				}
	
				Image(ImageType &&image, vk::raii::ImageView &&imageView);

				Image(Image&& image) noexcept;

				void setImageView(vk::raii::Device& device, vk::Format format);

				vk::MemoryRequirements getImageMemoryReq();

				void transitionImageLayout(vk::ImageLayout oldLayout, vk::ImageLayout newLayout, vk::AccessFlags2 srcAccessMask, vk::AccessFlags2 dstAccessMask, vk::PipelineStageFlags2 srcStageMask, vk::PipelineStageFlags2 dstStageMask, vk::ImageAspectFlags aspectFlag, vk::raii::CommandBuffer& commandBuff);

				ImageType& imageRef();

				~Image() = default;
		};

		template<typename ImageType>
		class ImageCollection {

			private:
				std::vector<Image<ImageType>> mImageCollection;
				std::size_t mSize{};
				
			public:

				inline std::size_t getSize() {
					return mSize;
				}

				void clear();
				void addImagesWithView(std::vector<ImageType> &&images, vk::Format format, vk::raii::Device& device);

				Image<ImageType>& back() {
					return mImageCollection.back();
				}

				void addOwnedImage(vk::Format format, vk::raii::Device& device, vk::Extent3D extents, vk::ImageTiling tiling, vk::ImageUsageFlags usageFlags);



				inline Image<ImageType>& getImage(uint32_t imageIdx) {
					return mImageCollection[imageIdx];
				}
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