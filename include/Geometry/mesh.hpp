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
				std::vector<uint32_t> mLocalIndexData;
				Core::Memory mAllocatedVertexMem;
				Core::Memory mAllocatedIndicesMem;

			public:
				vk::Buffer getUnderlyingBuffer();
				MeshManager(Core::DeviceMemoryManager& memManager);
				void addVertices(std::initializer_list<Vertex>&& initList);
				void submitMesh(Core::VulkanContext& vulkanCtx);
				inline uint32_t getVertexCount() {
					return mLocalVerticesData.size();
				}

				void addIndices(std::initializer_list<uint32_t>&& initList);

				inline uint32_t getSizeOfLocalInBytes() {
					return mLocalVerticesData.size() * sizeof(*mLocalVerticesData.begin());
				}

				inline Core::Memory getVertexMemoryInfo() {
					return mAllocatedVertexMem;
				}

				inline Core::Memory getIndicesMemoryInfo() {
					return mAllocatedIndicesMem;
				}

		};


	}
}


#endif