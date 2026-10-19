#ifndef RESOURCES_UPLOADABLE_IMAGE_HPP
#define RESOURCES_UPLOADABLE_IMAGE_HPP

#include "uploadableResource.hpp"
#include "deviceMemManager.hpp"
#include "ownershipTransfer.hpp"
#include "stagingBuffer.hpp"

namespace Celer {

	namespace Resource {

		class ImageTransfer : public OwnershipTransfer {
			private:
				vk::Image mImageHandle;
			public:
				ImageTransfer(vk::Image image);
				void acquire(Wrapper::CommandBuffer& commandBuff, Core::VulkanContext& ctx);
				static std::unique_ptr<ImageTransfer> create(vk::Image image);
		};



		class UploadableImage : public UploadableResource {
			private:
				uint32_t mWidth{};
				uint32_t mHeight{};
				vk::Image mImageHandle;
				Core::Memory mMemory{};
				Wrapper::StagingBuffer<std::uint8_t> mBuffer;

			public:
				UploadableImage(uint32_t width, uint32_t height, void* data, Core::VulkanContext& ctx, vk::Image image, Core::Memory memory);
				~UploadableImage() = default;
				virtual std::unique_ptr<OwnershipTransfer> upload(Wrapper::CommandBuffer& commandBuff, Core::VulkanContext& ctx) override;
				static std::unique_ptr<UploadableImage> create(uint32_t width, uint32_t height, void* data, Core::VulkanContext& ctx, vk::Image image, Core::Memory memory);

		};

	}

}



#endif // RESOURCES_UPLOADABLE_IMAGE_HPP