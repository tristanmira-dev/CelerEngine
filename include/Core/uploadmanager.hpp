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

		struct TransferInfo {
			uint32_t mPrevQueueIdx;
			uint32_t mNewQueueIdx;
			vk::raii::Image* mImageResource = nullptr;
			vk::raii::Buffer* mBufferResource = nullptr;
		};

		struct ResourceUploadInfo {
			ResourceType mResourceType;
			Memory mMemoryInfo;
			Wrapper::BufferStagingResource<stbi_uc> mBuffer;
			vk::raii::Image* mImageResource = nullptr;
			vk::raii::Buffer* mBufferResource = nullptr;

			ResourceUploadInfo(ResourceType resourceType, Memory memory, void* data, VulkanContext& ctx, vk::raii::Image* image = nullptr, vk::raii::Buffer* buffer = nullptr);
		};

		



		class UploadManager {
			private:
				std::vector<ResourceUploadInfo> mPendingUpload;
				std::vector<TransferInfo> mPendingTransfer;


				Wrapper::CommandBuffer mCommandBuff;


			public:

				UploadManager(VulkanContext& vulkanContext);

				void update(FrameContext& frameCtx, VulkanContext& ctx, DeviceMemoryManager& memoryManager);


				void addImageResource(VulkanContext& vulkanCtx, ResourceType resourceType, Memory memoryInfo, void* data, vk::raii::Image* image);



		};


	}


}


#endif // CORE_UPLOADMANAGER_HPP