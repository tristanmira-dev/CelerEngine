#ifndef IMAGE_HPP
#define IMAGE_HPP

namespace Celer {

	namespace Wrapper {

		void transitionImageLayout(vk::Image image, vk::ImageLayout oldLayout, vk::ImageLayout newLayout, vk::AccessFlags2 srcAccessMask, vk::AccessFlags2 dstAccessMask, vk::PipelineStageFlags2 srcStageMask, vk::PipelineStageFlags2 dstStageMask, vk::ImageAspectFlags aspectFlag, vk::raii::CommandBuffer& commandBuff);

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

		class ImageCollection {

			private:

				std::vector<RaiiImage> mImageCollection;

			public:

				
				ImageCollection() = default;
				~ImageCollection() = default;

				//DELETE THESE COPY ASSIGNMENT AND COPY CONSTRUCTORS CUZ OF RAII ELEMENTS OF STD VECTOR
				ImageCollection(ImageCollection const& src) = delete;
				ImageCollection& operator=(ImageCollection const& src) = delete;
				//

				//MOVE CONSTRUCTOR AND MOVE ASSIGNMENT
				ImageCollection(ImageCollection &&src) noexcept;
				ImageCollection& operator=(ImageCollection&& src) noexcept;
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

	}
}

#endif // IMAGE_HPP