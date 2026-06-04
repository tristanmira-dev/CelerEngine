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
				MeshManager(Core::DeviceMemoryManager& memManager);
				void addMesh(std::initializer_list<Vertex>&& initList);
				void submitMesh();

		};


	}
}


#endif