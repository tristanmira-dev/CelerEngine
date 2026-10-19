#ifndef CORE_UPLOADMANAGER_HPP
#define CORE_UPLOADMANAGER_HPP

#include "deviceMemManager.hpp"
#include "stagingBuffer.hpp"
#include "sync.hpp"
#include "uploadableResource.hpp"
#include "ownershipTransfer.hpp"
#include <stb_image.h>
#include <memory>

namespace Celer {

	namespace Core {

		class UploadManager {
			private:
				std::vector<std::unique_ptr<Resource::UploadableResource>> mPendingUpload;
				std::vector<std::unique_ptr<Resource::OwnershipTransfer>> mPendingAcquire;

				Wrapper::CommandBuffer mTransferCommandBuff;
				Wrapper::CommandBuffer mGraphicsCommandBuff;

				vk::raii::Fence mUploadFence = nullptr;

				Wrapper::StagingBuffer<uint8_t> mSharedStagingBuffer;

				void upload(FrameContext& frameCtx, VulkanContext& ctx);

				void acquire(FrameContext& frameCtx, VulkanContext& ctx);


			public:

				UploadManager(VulkanContext& vulkanContext);

				void update(FrameContext& frameCtx, VulkanContext& ctx, DeviceMemoryManager& memoryManager);

				void queueUploadResource(std::unique_ptr<Resource::UploadableResource> &&uploadable);

		};


	}


}


#endif // CORE_UPLOADMANAGER_HPP