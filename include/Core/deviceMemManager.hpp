#ifndef DEVICEMEMMANAGER_HPP
#define DEVICEMEMMANAGER_HPP

#include "vulkanBuffer.hpp"
#include "vertex.hpp"

namespace Celer {

	namespace Core {
		class DeviceMemoryManager {
			private:
				/*Vertex Buff*/
				uint32_t mVertexMemSize{};
				Wrapper::Buffer mVertexBuffer;
				std::vector<Geometry::Vertex> mVertexStagingBuffer;

			public:

			
			

				DeviceMemoryManager(Core::VulkanContext const& vulkanCtx);
		};
	}

}

#endif // DEVICEMEMMANAGER_HPP