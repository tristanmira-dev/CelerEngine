#include "deviceMemManager.hpp"
namespace Celer {
	namespace Core {


		uint8_t* DeviceMemoryManager::getDescriptorMappedMemory(uint32_t offset) {


			uint8_t* memory{ static_cast<uint8_t*>(mDescriptorMappedBuff) };

			return memory + offset;

		}

		vk::Buffer DeviceMemoryManager::getDescriptorBuffer() {
			return mDescriptorBuffer.getBuffer();
		}


		void DeviceMemoryManager::createImageDeviceBuffer(VulkanContext& vulkanCtx, vk::DeviceSize deviceSize) {
		
			/*TODO STORE THIS SHET*/
			vk::ImageCreateInfo imageInfo{ 
				.imageType = vk::ImageType::e2D,
				.format = vk::Format::eR8G8B8A8Srgb,
				.extent = {1, 1, 1},
				.mipLevels = 1,
				.arrayLayers = 1,
				.samples = vk::SampleCountFlagBits::e1,
				.tiling = vk::ImageTiling::eOptimal,
				.usage = vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eSampled,
				.sharingMode = vk::SharingMode::eExclusive 
			};

			vk::raii::Image tempImg(*vulkanCtx.device, imageInfo);

			vk::MemoryRequirements imgReq{ tempImg.getMemoryRequirements() };

			vk::MemoryAllocateInfo memoryInfo{ .allocationSize = deviceSize,
										 .memoryTypeIndex = findMemoryType(imgReq.memoryTypeBits, vk::MemoryPropertyFlagBits::eDeviceLocal, *vulkanCtx.physicalDevice) };
			
			mImageDeviceMemory = vk::raii::DeviceMemory(*vulkanCtx.device, memoryInfo);
		}




		void DeviceMemoryManager::bindImage(vk::raii::Image const& img, Memory const& memory) {
			img.bindMemory(mImageDeviceMemory, memory.getOffset());
		}

		uint32_t DeviceMemoryManager::getAlignedOffset(uint32_t bytes, uint32_t alignTo) {
		
			while (bytes % alignTo != 0) ++bytes;

			return bytes;
		
		}



		DeviceMemoryManager::DeviceMemoryManager(Core::VulkanContext& vulkanCtx) :
			mMainBufferSize{ 1024 * 1024 * 500 },
			mMainBuffer(mMainBufferSize, vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eVertexBuffer | vk::BufferUsageFlagBits::eIndexBuffer | vk::BufferUsageFlagBits::eStorageBuffer | vk::BufferUsageFlagBits::eUniformBuffer, vk::MemoryPropertyFlagBits::eDeviceLocal, vulkanCtx),
			mStagingBuffer(1024 * 1024 * 64, vulkanCtx),
			mMappedStagingBuff{ mStagingBuffer.getMappedMemory() },

			mDescriptorBuffer(1024 * 1024 * 500, vk::BufferUsageFlagBits::eUniformBuffer | vk::BufferUsageFlagBits::eStorageBuffer, vulkanCtx),
			mDescriptorMappedBuff{ mDescriptorBuffer.getMappedMemory() },

			mTransferQueue{ *vulkanCtx.transferQueue },
			mTransferQueueIdx{ vulkanCtx.transferQueueIdx },
			mCommandBuffer(*vulkanCtx.device, 1, vulkanCtx.transferQueueIdx),
			mCurrentMainBufferSize{mMainBufferSize}
		
		{
			mMainBuffer.setQueueOwner(mTransferQueueIdx);
			mStagingBuffer.setQueueOwner(mTransferQueueIdx);

			mTransferFence = vk::raii::Fence(*vulkanCtx.device, vk::FenceCreateInfo{.flags = vk::FenceCreateFlagBits::eSignaled});

			mTransferFinished = vk::raii::Semaphore(*vulkanCtx.device, vk::SemaphoreCreateInfo{});

			createImageDeviceBuffer(vulkanCtx, 1024 * 1024 * 500);

			std::cout << "SANITY CHECK TRANSFER: " << vulkanCtx.transferQueueIdx << " GRAPHICS: " << vulkanCtx.graphicsQueueIdx << '\n';




		}

		DeviceMemoryManager::~DeviceMemoryManager() {
			
		
		}


	}
}
