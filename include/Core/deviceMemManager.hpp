#ifndef DEVICEMEMMANAGER_HPP
#define DEVICEMEMMANAGER_HPP

#include "vulkanBuffer.hpp"
#include "vertex.hpp"
#include "contexts.hpp"
#include "commandBuffer.hpp"
#include <list>

namespace Celer {

	namespace Core {


		class Memory {
			private:
				uint32_t mOffset;
				uint32_t mSize; //in bytes
				bool mIsFree{ true };

			public:
				Memory() = default;
				Memory(uint32_t offset, uint32_t size, bool isFree) : mOffset{ offset }, mSize{ size }, mIsFree{ isFree } {}

				inline uint32_t getSize() {
					return mSize;
				}

				inline uint32_t getOffset() {
					return mOffset;
				}
		};


		class DeviceMemoryManager {
			private:
				uint32_t mMainBufferSize;

				Wrapper::Buffer mMainBuffer;
				Wrapper::Buffer mStagingBuffer;
				void* mMappedStagingBuff;

				std::list<Memory> mMemoryTracker;

				/*Queue*/
				vk::raii::Queue& mTransferQueue;
				Wrapper::CommandBuffer mCommandBuffer;



			public:

				void transferMemoryToLocalBuffer(VulkanContext& vulkanCtx, Wrapper::Buffer& src, Wrapper::Buffer& dst, vk::DeviceSize size);

				Memory allocateMemory(uint32_t size);

				template<typename Iterable>
				void addToDeviceBuffer(Iterable& container) {
					//probably need to handle a case to do this in batches, cuz mStagingBuffer is less than the MainBuffer and what if container exceeds this (hypothetical)
					memcpy(mMappedStagingBuff, container.data(), container.size() * sizeof(*container.begin()));
				}

				DeviceMemoryManager(VulkanContext& vulkanCtx);

				~DeviceMemoryManager();
		};

	}

}

#endif // DEVICEMEMMANAGER_HPP