#include "deviceMemManager.hpp"
namespace Celer {
	namespace Core {

		
		//VertexBuffer::VertexBuffer(VulkanContext vulkanCtx) :
		//	mVertexMemSize{ 1024 * 1024 * 500 },
		//	mVertexBuffer(mVertexMemSize,
		//	vk::BufferUsageFlagBits::eVertexBuffer | vk::BufferUsageFlagBits::eTransferDst, vk::MemoryPropertyFlagBits::eDeviceLocal, vulkanCtx) {

		//}

		void DeviceMemoryManager::transferOwnership(VulkanContext &vulkanCtx, uint32_t oldQueueIdx, uint32_t newQueueIdx) {

			vulkanCtx.device->waitForFences(*mTransferFence, vk::True, UINT64_MAX);
			vulkanCtx.device->resetFences(*mTransferFence);

			mCommandBuffer.getSingleBuffer().reset();

			uint32_t mainBuffCurrentOwner{ mMainBuffer.getQueueOwner() };
			

			if (mainBuffCurrentOwner == newQueueIdx) return;

			mMainBuffer.setQueueOwner(newQueueIdx);

			mCommandBuffer.beginSingleTimeCommand();
			vk::BufferMemoryBarrier release{
					.srcAccessMask = vk::AccessFlagBits::eTransferWrite,
					.dstAccessMask = vk::AccessFlagBits::eNone,
					.srcQueueFamilyIndex = mTransferQueueIdx,
					.dstQueueFamilyIndex = vulkanCtx.graphicsQueueIdx,
					.buffer = mMainBuffer.getBuffer(),
					.offset = 0,
					.size = mMainBuffer.getSize()
			};

			mCommandBuffer.getSingleBuffer().pipelineBarrier(vk::PipelineStageFlagBits::eTransfer, vk::PipelineStageFlagBits::eBottomOfPipe, {}, {}, release, {});

			mCommandBuffer.endSyncCommand(mTransferQueue, 0, nullptr, &mTransferFinished, vk::PipelineStageFlagBits::eBottomOfPipe);



		}


		void DeviceMemoryManager::transferMemoryToLocalBuffer(VulkanContext& vulkanCtx, Memory const &memory, std::size_t dataSize) {

			vulkanCtx.device->waitForFences(*mTransferFence, vk::True, UINT64_MAX);
			vulkanCtx.device->resetFences(*mTransferFence);

			mCommandBuffer.getSingleBuffer().reset();

			uint32_t mainBuffCurrentOwner{ mMainBuffer.getQueueOwner() };

			if (mainBuffCurrentOwner == mTransferQueueIdx) {
				mCommandBuffer.beginSingleTimeCommand();
				mCommandBuffer.getSingleBuffer().copyBuffer(mStagingBuffer.getBuffer(), mMainBuffer.getBuffer(), vk::BufferCopy(0, memory.getOffset(), dataSize));
				vk::BufferMemoryBarrier release{
					.srcAccessMask = vk::AccessFlagBits::eTransferWrite,
					.dstAccessMask = vk::AccessFlagBits::eNone,
					.srcQueueFamilyIndex = mTransferQueueIdx,
					.dstQueueFamilyIndex = vulkanCtx.graphicsQueueIdx,
					.buffer = mMainBuffer.getBuffer(),
					.offset = 0,
					.size = mMainBuffer.getSize()
				};

				mCommandBuffer.endSyncCommand(mTransferQueue, 0, &mTransferFinished, nullptr, vk::PipelineStageFlagBits::eTopOfPipe, &mTransferFence);

			} else {

			}

			
		}

		uint32_t DeviceMemoryManager::getAlignedOffset(uint32_t bytes, uint32_t alignTo) {
		
			while (bytes % alignTo != 0) ++bytes;

			return bytes;
		
		}

		void DeviceMemoryManager::mainBuffAcquireQueueOwnership(Wrapper::CommandBuffer& commandBuffer, uint32_t commandBufferIdx, uint32_t oldOwnerIdx, uint32_t newOwnerIdx, vk::raii::Queue &queue, vk::raii::Fence &fence) {
			commandBuffer.getCommandBuffer(commandBufferIdx).begin({});

			commandBuffer.getCommandBuffer(commandBufferIdx).pipelineBarrier(vk::PipelineStageFlagBits::eTopOfPipe, vk::PipelineStageFlagBits::eVertexInput, {}, {}, vk::BufferMemoryBarrier{
				.srcAccessMask = vk::AccessFlagBits::eNone,
				.dstAccessMask = vk::AccessFlagBits::eVertexAttributeRead,
				.srcQueueFamilyIndex = oldOwnerIdx,
				.dstQueueFamilyIndex = newOwnerIdx,
				.buffer = mMainBuffer.getBuffer(),
				.offset = 0,
				.size = mMainBuffer.getSize()
				
			}, {});

			commandBuffer.endSyncCommand(queue, commandBufferIdx, nullptr, &mTransferFinished, vk::PipelineStageFlagBits::eVertexInput, &fence);

			mMainBuffer.setQueueOwner(newOwnerIdx);
		}

		DeviceMemoryManager::DeviceMemoryManager(Core::VulkanContext& vulkanCtx) :
			mMainBufferSize{ 1024 * 1024 * 500 },
			mMainBuffer(Wrapper::Buffer::createDeviceLocalBuffer(mMainBufferSize, vulkanCtx, vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eVertexBuffer | vk::BufferUsageFlagBits::eIndexBuffer | vk::BufferUsageFlagBits::eStorageBuffer | vk::BufferUsageFlagBits::eUniformBuffer)),
			mMappedStagingBuff{ mStagingBuffer.mapMemory() },
			mStagingBuffer(1024 * 1024 * 64, vk::BufferUsageFlagBits::eTransferSrc, vk::MemoryPropertyFlagBits::eHostCoherent | vk::MemoryPropertyFlagBits::eHostVisible, vulkanCtx),
			mTransferQueue{ *vulkanCtx.transferQueue },
			mTransferQueueIdx{ vulkanCtx.transferQueueIdx },
			mCommandBuffer(*vulkanCtx.device, 1, vulkanCtx.transferQueueIdx)
		
		{
			mMainBuffer.setQueueOwner(mTransferQueueIdx);
			mStagingBuffer.setQueueOwner(mTransferQueueIdx);

			mTransferFence = vk::raii::Fence(*vulkanCtx.device, vk::FenceCreateInfo{.flags = vk::FenceCreateFlagBits::eSignaled});

			mTransferFinished = vk::raii::Semaphore(*vulkanCtx.device, vk::SemaphoreCreateInfo{});

			std::cout << "SANITY CHECK TRANSFER: " << vulkanCtx.transferQueueIdx << " GRAPHICS: " << vulkanCtx.graphicsQueueIdx << '\n';
			//allocateMemory(4);

			//std::vector<Geometry::Vertex> test{{{1.f, 1.f, 1.f}, { 1.f, 1.f, 1.f }, { 1.f, 1.f }}, };
			//addToDeviceBuffer(test);




		}

		DeviceMemoryManager::~DeviceMemoryManager() {
			
			mStagingBuffer.unmapMemory();
		
		}


	}
}
