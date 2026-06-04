#include "deviceMemManager.hpp"
namespace Celer {
	namespace Core {

		
		//VertexBuffer::VertexBuffer(VulkanContext vulkanCtx) :
		//	mVertexMemSize{ 1024 * 1024 * 500 },
		//	mVertexBuffer(mVertexMemSize,
		//	vk::BufferUsageFlagBits::eVertexBuffer | vk::BufferUsageFlagBits::eTransferDst, vk::MemoryPropertyFlagBits::eDeviceLocal, vulkanCtx) {

		//}


		void DeviceMemoryManager::transferMemoryToLocalBuffer(VulkanContext& vulkanCtx, Memory const &memory, std::size_t dataSize) {

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

				mCommandBuffer.getSingleBuffer().pipelineBarrier(vk::PipelineStageFlagBits::eTransfer, vk::PipelineStageFlagBits::eBottomOfPipe, {}, {}, release, {});

				mCommandBuffer.endSingleTimeCommand(mTransferQueue, &mTransferFinished);

			} else {

			}

			
		}

		void DeviceMemoryManager::mainBuffAcquireQueueOwnership(Wrapper::CommandBuffer& commandBuffer, uint32_t oldOwnerIdx, uint32_t newOwnerIdx, vk::raii::Queue &queue) {
			commandBuffer.beginSingleTimeCommand();

			commandBuffer.getSingleBuffer().pipelineBarrier(vk::PipelineStageFlagBits::eTopOfPipe, vk::PipelineStageFlagBits::eVertexInput, {}, {}, vk::BufferMemoryBarrier {
				.srcAccessMask = vk::AccessFlagBits::eNone,
				.dstAccessMask = vk::AccessFlagBits::eVertexAttributeRead,
				.srcQueueFamilyIndex = oldOwnerIdx,
				.dstQueueFamilyIndex = newOwnerIdx,
				.buffer = mMainBuffer.getBuffer(),
				.offset = 0,
				.size = mMainBuffer.getSize()
				
			}, {});

			commandBuffer.endSingleTimeCommand(queue);

			mMainBuffer.setQueueOwner(newOwnerIdx);
		}

		Memory DeviceMemoryManager::allocateMemory(uint32_t size) {

			if (mMemoryTracker.size() == 0) {
				mMemoryTracker.push_back(Memory{ 0, size, false });
				return mMemoryTracker.back();

			}

			Memory backIter{ mMemoryTracker.back() };

			mMemoryTracker.push_back(Memory{ backIter.getOffset() + backIter.getSize(), size, false });
			return mMemoryTracker.back();

		}

		DeviceMemoryManager::DeviceMemoryManager(Core::VulkanContext& vulkanCtx) :
			mMainBufferSize{ 1024 * 1024 * 500 },
			mMainBuffer(Wrapper::Buffer::createDeviceLocalBuffer(mMainBufferSize, vulkanCtx, vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eVertexBuffer | vk::BufferUsageFlagBits::eIndexBuffer)),
			mMappedStagingBuff{ mStagingBuffer.mapMemory() },
			mStagingBuffer(1024 * 1024 * 64, vk::BufferUsageFlagBits::eTransferSrc, vk::MemoryPropertyFlagBits::eHostCoherent | vk::MemoryPropertyFlagBits::eHostVisible, vulkanCtx),
			mTransferQueue{ *vulkanCtx.transferQueue },
			mTransferQueueIdx{ vulkanCtx.transferQueueIdx },
			mCommandBuffer(*vulkanCtx.device, 1, vulkanCtx.transferQueueIdx)
		
		{
			mMainBuffer.setQueueOwner(mTransferQueueIdx);
			mStagingBuffer.setQueueOwner(mTransferQueueIdx);

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
