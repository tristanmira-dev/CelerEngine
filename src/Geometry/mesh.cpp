#include "mesh.hpp"


namespace Celer {
	namespace Geometry {

		
		MeshManager::MeshManager(Core::DeviceMemoryManager& memManager) : mMemManager{ memManager } {
			mAllocatedGPUMem = mMemManager.allocateMemory(1024 * 1024 * 100);
		}

		void MeshManager::addMesh(std::initializer_list<Vertex> &&initList) {
			std::copy(initList.begin(), initList.end(), std::back_inserter(mLocalVerticesData));
			//mLocalVerticesData.push_back(Vertex{ glm::vec3{1.f, 1.f, 1.f}, glm::vec3{0.f,0.f,0.f}, glm::vec2{1.f, 1.f} });
		}

		void MeshManager::submitMesh() {
			mMemManager.addToDeviceBuffer(mLocalVerticesData);

		}



	}
}
