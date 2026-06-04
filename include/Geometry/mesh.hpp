#ifndef MESH_HPP
#define MESH_HPP

#include "vertex.hpp"
#include "deviceMemManager.hpp"

namespace Celer {
	namespace Geometry {

		class MeshManager {
			private:
				Core::DeviceMemoryManager& mMemManager;
				std::vector<Vertex> mLocalVerticesData;
				Core::Memory mAllocatedGPUMem;

			public:
				vk::Buffer getUnderlyingBuffer();
				MeshManager(Core::DeviceMemoryManager& memManager);
				void addMesh(std::initializer_list<Vertex>&& initList);
				void submitMesh(Core::VulkanContext& vulkanCtx);
				inline uint32_t getVertexCount() {
					return mLocalVerticesData.size();
				}

				inline uint32_t getSizeOfLocalInBytes() {
					return mLocalVerticesData.size() * sizeof(*mLocalVerticesData.begin());
				}

				inline Core::Memory getMemoryInfo() {
					return mAllocatedGPUMem;
				}

		};


	}
}


#endif