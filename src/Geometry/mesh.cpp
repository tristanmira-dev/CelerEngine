#include "mesh.hpp"
#include "uploadableBuffer.hpp"


namespace Celer {
	namespace Geometry {
		vk::Buffer MeshManager::getUnderlyingBuffer() {
			return mMemManager.getMainBuffer();
		}
		MeshManager::MeshManager(Core::DeviceMemoryManager& memManager) : mMemManager{ memManager } {
			mAllocatedVertexMem = mMemManager.allocateMemory<Vertex>(1024 * 1024 * 100);
			mAllocatedIndicesMem = mMemManager.allocateMemory<uint32_t>(1024 * 1024 * 100);
			mBufferHandle = memManager.getMainBuffer();
		}

		MeshInfo MeshManager::getMeshInfo(uint32_t idx) {
			return mMeshInfo[idx];
		}

		void MeshManager::addVertices(std::initializer_list<Vertex> &&initList) {
			std::copy(initList.begin(), initList.end(), std::back_inserter(mLocalVerticesData));
		}

		void MeshManager::addIndices(std::initializer_list<uint32_t>&& initList) {
			std::copy(initList.begin(), initList.end(), std::back_inserter(mLocalIndexData));

			if (!mMeshInfo.size()) mMeshInfo.push_back(MeshInfo{ .offset = 0, .indicesCount = static_cast<uint32_t>(initList.size()) });
			else {

				MeshInfo info{ mMeshInfo.back() };
				mMeshInfo.push_back(MeshInfo{ .offset = info.offset + info.indicesCount, .indicesCount = static_cast<uint32_t>(initList.size()) });

			}
		}


		void MeshManager::submitMesh(Core::VulkanContext& vulkanCtx, Core::UploadManager& uploadManager) {
			uploadManager.queueUploadResource(Resource::UploadableBuffer::create(vulkanCtx, mBufferHandle, mAllocatedVertexMem, mLocalVerticesData.data(), sizeof(mLocalVerticesData[0]) * mLocalVerticesData.size()));
		}


		void MeshManager::submitIndices(Core::VulkanContext& vulkanCtx, Core::UploadManager& uploadManager) {
			uploadManager.queueUploadResource(Resource::UploadableBuffer::create(vulkanCtx, mBufferHandle, mAllocatedIndicesMem, mLocalIndexData.data(), sizeof(mLocalIndexData[0]) * mLocalIndexData.size()));

		}



	}
}
