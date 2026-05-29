#include "deviceMemManager.hpp"
namespace Celer {
	namespace Core {
		DeviceMemoryManager::DeviceMemoryManager(Core::VulkanContext const &vulkanCtx) : mVertexMemSize{1024 * 1024 * 500}, mVertexBuffer(mVertexMemSize, vk::BufferUsageFlagBits::eVertexBuffer | vk::BufferUsageFlagBits::eTransferDst, vk::MemoryPropertyFlagBits::eDeviceLocal, vulkanCtx) {
		

		}
	}
}
