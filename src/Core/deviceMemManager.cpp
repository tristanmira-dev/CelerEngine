#include "deviceMemManager.hpp"
namespace Celer {
	namespace Core {

		
		//VertexBuffer::VertexBuffer(VulkanContext vulkanCtx) :
		//	mVertexMemSize{ 1024 * 1024 * 500 },
		//	mVertexBuffer(mVertexMemSize,
		//	vk::BufferUsageFlagBits::eVertexBuffer | vk::BufferUsageFlagBits::eTransferDst, vk::MemoryPropertyFlagBits::eDeviceLocal, vulkanCtx) {

		//}


		void DeviceMemoryManager::transferMemoryToLocalBuffer(VulkanContext& vulkanCtx, Wrapper::Buffer &src, Wrapper::Buffer &dst) {
			mCommandBuffer.beginSingleTimeCommand();
			mCommandBuffer.getSingleBuffer().copyBuffer(src.getBuffer(), dst.getBuffer(), nullptr);
			mCommandBuffer.endSingleTimeCommand(mTransferQueue);
		}

		DeviceMemoryManager::DeviceMemoryManager(Core::VulkanContext& vulkanCtx) :
			mVertBuffer(vulkanCtx, 1024*1024*500),
			mTransferQueue{ *vulkanCtx.transferQueue },
			mCommandBuffer(*vulkanCtx.device, 1, vulkanCtx.transferQueueIdx)
		
		{
			

		}


	}
}
