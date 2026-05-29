#ifndef DEVICEMEMMANAGER_HPP
#define DEVICEMEMMANAGER_HPP

#include "vulkanBuffer.hpp"
#include "vertex.hpp"
#include "contexts.hpp"
#include "commandBuffer.hpp"

namespace Celer {

	namespace Core {


		


		class DeviceMemoryManager {
			private:
				Wrapper::BufferResource<Geometry::Vertex> mVertBuffer;

				/*Queue*/
				vk::raii::Queue& mTransferQueue;

				Wrapper::CommandBuffer mCommandBuffer;

			public:

				void transferMemoryToLocalBuffer(VulkanContext& vulkanCtx, Wrapper::Buffer& src, Wrapper::Buffer& dst);

				DeviceMemoryManager(VulkanContext& vulkanCtx);
		};

	}

}

#endif // DEVICEMEMMANAGER_HPP