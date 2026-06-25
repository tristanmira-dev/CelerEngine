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
				void submitMesh(Core::VulkanContext& vulkanCtx, bool endOfBatch = true);
				void submitIndices(Core::VulkanContext& vulkanCtx, bool endOfBatch = true);
				inline uint32_t getVertexCount() {
					return mLocalVerticesData.size();
				}

				inline uint32_t getIndicesCount() {
					return static_cast<uint32_t>(mLocalIndexData.size());
				}

				void addIndices(std::initializer_list<uint32_t>&& initList);

				template<typename T>
				inline uint32_t getSizeOfLocalInBytes(std::vector<T> data) {
					return data.size() * sizeof(*data.begin());
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