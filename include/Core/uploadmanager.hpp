#ifndef CORE_UPLOADMANAGER_HPP
#define CORE_UPLOADMANAGER_HPP



#include "deviceMemManager.hpp"
#include "stagingBuffer.hpp"
#include "sync.hpp"
#include <stb_image.h>
#include <memory>

namespace Celer {

	namespace Core {

		enum ResourceType {
			BUFFER,
			IMAGE
		};

		struct ResourceAcquireInfo {
			uint32_t oldQueue;
			uint32_t newQueue;
			vk::Image mImageResource;
			vk::Buffer mBufferResource;
		};

		//TODO ADD A MEMBER TO SPECIFY WHICH QUEUE WILL USE THE RESOURCE
		struct ResourceUploadInfo {
			ResourceType mResourceType;
			Memory mMemoryInfo;
			Wrapper::StagingBuffer<std::uint8_t> mBuffer;
			vk::Image mImageResource;
			vk::Buffer mBufferResource;
			uint32_t mWidth;
			uint32_t mHeight;

			ResourceUploadInfo(ResourceType resourceType, Memory memory, void* data, VulkanContext& ctx, vk::Image image, vk::Buffer buffer, uint32_t width, uint32_t height);
		};

		
		class UploadableResource {
			public:
				virtual ~UploadableResource() = default;
				UploadableResource() = default;
				UploadableResource(UploadableResource const&) = delete;
				UploadableResource& operator=(UploadableResource const&) = delete;

				virtual void upload(vk::raii::CommandBuffer& commandBuff) = 0;
		};

		class UploadableImage : public UploadableResource {
			private:
				uint32_t mWidth;
				uint32_t mHeight;
				Wrapper::StagingBuffer<std::uint8_t> mBuffer;

			public:
				~UploadableImage();
				virtual void upload(vk::raii::CommandBuffer& commandBuff) override;
		};



		class UploadManager {
			private:
				std::vector<ResourceUploadInfo> mPendingUpload;
				std::vector<ResourceAcquireInfo> mPendingAcquire;

				Wrapper::CommandBuffer mTransferCommandBuff;
				Wrapper::CommandBuffer mGraphicsCommandBuff;

				vk::raii::Fence mUploadFence = nullptr;

				Wrapper::StagingBuffer<uint8_t> mSharedStagingBuffer;

				void addAcquire(ResourceAcquireInfo const& info);

				void upload(FrameContext& frameCtx, VulkanContext& ctx);

				void acquire(FrameContext& frameCtx, VulkanContext& ctx);


			public:

				UploadManager(VulkanContext& vulkanContext);

				void update(FrameContext& frameCtx, VulkanContext& ctx, DeviceMemoryManager& memoryManager);

				void addBufferResource(VulkanContext& vulkanCtx, ResourceType resourceType, Memory memoryInfo, vk::Buffer buffer);

				void addImageResource(VulkanContext& vulkanCtx, ResourceType resourceType, Memory memoryInfo, void* data, vk::Image image, uint32_t width, uint32_t height);



		};


	}


}


#endif // CORE_UPLOADMANAGER_HPP