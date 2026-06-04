#include "deviceMemManager.hpp"
namespace Celer {
	namespace Core {

		
		//VertexBuffer::VertexBuffer(VulkanContext vulkanCtx) :
		//	mVertexMemSize{ 1024 * 1024 * 500 },
		//	mVertexBuffer(mVertexMemSize,
		//	vk::BufferUsageFlagBits::eVertexBuffer | vk::BufferUsageFlagBits::eTransferDst, vk::MemoryPropertyFlagBits::eDeviceLocal, vulkanCtx) {

		//}


		void DeviceMemoryManager::transferMemoryToLocalBuffer(VulkanContext& vulkanCtx, Wrapper::Buffer &src, Wrapper::Buffer &dst, vk::DeviceSize size) {
			mCommandBuffer.beginSingleTimeCommand();
			mCommandBuffer.getSingleBuffer().copyBuffer(src.getBuffer(), dst.getBuffer(), vk::BufferCopy(0, 0, size));
			mCommandBuffer.endSingleTimeCommand(mTransferQueue);
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
			mMainBufferSize{1024*1024*500},
			mMainBuffer(Wrapper::Buffer::createDeviceLocalBuffer(mMainBufferSize, vulkanCtx, vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eVertexBuffer | vk::BufferUsageFlagBits::eIndexBuffer)),
			mMappedStagingBuff{mStagingBuffer.mapMemory()},
			mStagingBuffer(1024 * 1024 * 64, vk::BufferUsageFlagBits::eTransferSrc, vk::MemoryPropertyFlagBits::eHostCoherent | vk::MemoryPropertyFlagBits::eHostVisible, vulkanCtx),
			mTransferQueue{ *vulkanCtx.transferQueue },
			mCommandBuffer(*vulkanCtx.device, 1, vulkanCtx.transferQueueIdx)
		
		{


			
			//allocateMemory(4);

			//std::vector<Geometry::Vertex> test{{{1.f, 1.f, 1.f}, { 1.f, 1.f, 1.f }, { 1.f, 1.f }}, };
			//addToDeviceBuffer(test);




		}

		DeviceMemoryManager::~DeviceMemoryManager() {
			
			mStagingBuffer.unmapMemory();
		
		}


	}
}
