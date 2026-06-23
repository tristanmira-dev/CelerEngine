#include "mesh.hpp"


namespace Celer {
	namespace Geometry {
		vk::Buffer MeshManager::getUnderlyingBuffer() {
			return mMemManager.getMainBuffer();
		}
		MeshManager::MeshManager(Core::DeviceMemoryManager& memManager) : mMemManager{ memManager } {
			mAllocatedVertexMem = mMemManager.allocateMemory<Vertex>(1024 * 1024 * 100);
			mAllocatedIndicesMem = mMemManager.allocateMemory<uint32_t>(1024 * 1024 * 100);
		}

		void MeshManager::addVertices(std::initializer_list<Vertex> &&initList) {
			std::copy(initList.begin(), initList.end(), std::back_inserter(mLocalVerticesData));
			//mLocalVerticesData.push_back(Vertex{ glm::vec3{1.f, 1.f, 1.f}, glm::vec3{0.f,0.f,0.f}, glm::vec2{1.f, 1.f} });
		}

		void MeshManager::addIndices(std::initializer_list<uint32_t>&& initList) {
			std::copy(initList.begin(), initList.end(), std::back_inserter(mLocalIndexData));
			//mLocalVerticesData.push_back(Vertex{ glm::vec3{1.f, 1.f, 1.f}, glm::vec3{0.f,0.f,0.f}, glm::vec2{1.f, 1.f} });
		}


		void MeshManager::submitMesh(Core::VulkanContext& vulkanCtx) {
			mMemManager.addToDeviceBuffer(mLocalVerticesData);
			mMemManager.transferMemoryToLocalBuffer(vulkanCtx, mAllocatedVertexMem, getSizeOfLocalInBytes());
		}



	}
}
