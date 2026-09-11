#ifndef CORE_UPLOADMANAGER_HPP
#define CORE_UPLOADMANAGER_HPP



#include "deviceMemManager.hpp"
#include "sync.hpp"
#include <stb_image.h>

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
			Wrapper::BufferStagingResource<stbi_uc> mBuffer;
			vk::Image mImageResource;
			vk::Buffer mBufferResource;
			uint32_t mWidth;
			uint32_t mHeight;

			ResourceUploadInfo(ResourceType resourceType, Memory memory, void* data, VulkanContext& ctx, vk::Image image, vk::Buffer buffer, uint32_t width, uint32_t height);
		};

		



		class UploadManager {
			private:
				std::vector<ResourceUploadInfo> mPendingUpload;
				std::vector<ResourceAcquireInfo> mPendingAcquire;


				Wrapper::CommandBuffer mCommandBuff;

				Wrapper::CommandBuffer mGraphicsCommandBuff;

				vk::raii::Fence mUploadFence = nullptr;

				void addAcquire(ResourceAcquireInfo const& info);

				void upload(FrameContext& frameCtx, VulkanContext& ctx);

				void acquire(FrameContext& frameCtx, VulkanContext& ctx);


			public:

				UploadManager(VulkanContext& vulkanContext);

				void update(FrameContext& frameCtx, VulkanContext& ctx, DeviceMemoryManager& memoryManager);


				void addImageResource(VulkanContext& vulkanCtx, ResourceType resourceType, Memory memoryInfo, void* data, vk::Image image, uint32_t width, uint32_t height);



		};


	}


}


#endif // CORE_UPLOADMANAGER_HPP