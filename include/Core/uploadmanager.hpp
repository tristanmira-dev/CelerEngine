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
			vk::raii::Image* mImageResource = nullptr;
			vk::raii::Buffer* mBufferResource = nullptr;
		};

		//TODO ADD A MEMBER TO SPECIFY WHICH QUEUE WILL USE THE RESOURCE
		struct ResourceUploadInfo {
			ResourceType mResourceType;
			Memory mMemoryInfo;
			Wrapper::BufferStagingResource<stbi_uc> mBuffer;
			vk::raii::Image* mImageResource = nullptr;
			vk::raii::Buffer* mBufferResource = nullptr;
			uint32_t mWidth;
			uint32_t mHeight;

			ResourceUploadInfo(ResourceType resourceType, Memory memory, void* data, VulkanContext& ctx, vk::raii::Image* image = nullptr, vk::raii::Buffer* buffer = nullptr, uint32_t width = 0, uint32_t height = 0);
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


				void addImageResource(VulkanContext& vulkanCtx, ResourceType resourceType, Memory memoryInfo, void* data, vk::raii::Image* image, uint32_t width, uint32_t height);



		};


	}


}


#endif // CORE_UPLOADMANAGER_HPP