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

				inline uint32_t getSize() const {
					return mSize;
				}

				inline uint32_t getOffset() const {
					return mOffset;
				}

		};


		class DeviceMemoryManager {
			private:

				vk::raii::Fence mTransferFence{ nullptr }; //In progress to REMOVE!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! OFFLOAD TO UPLOAD MANAGER!

				uint32_t mMainBufferSize;

				/*Linear buffers*/
				Wrapper::Buffer mMainBuffer;
				Wrapper::Buffer mStagingBuffer;
				void* mMappedStagingBuff;
				std::list<Memory> mMemoryTracker; //Separate user defined class later probably (the logic for memory alloc)
				//

				/*Descriptor buffers*/
				Wrapper::Buffer mDescriptorBuffer;
				void* mDescriptorMappedBuff;
				std::list<Memory> mDescriptorMemoryTracker;
				//

				/*Queue*/
				vk::raii::Queue& mTransferQueue;
				uint32_t mTransferQueueIdx;
				//

				/*Sync Objects -- TODO, REMOVE LATER, OFFLOAD THIS TO THE UPLOAD MANAGER*/
				Wrapper::CommandBuffer mCommandBuffer;
				vk::raii::Semaphore mTransferFinished{ nullptr };
				//

				/*Non-linear buffers*/
				vk::raii::DeviceMemory mImageDeviceMemory{ nullptr };
				std::list<Memory> mImageMemoryTracker;
				//

				uint32_t mCurrentMainBufferSize;


			public:

				uint8_t* getDescriptorMappedMemory(uint32_t offset);

				vk::Buffer getDescriptorBuffer();
				
				void batchUpload(VulkanContext& vulkanCtx, Memory const& memory, std::size_t dataSize);

				void createImageDeviceBuffer(VulkanContext& vulkanCtx, vk::DeviceSize deviceSize);

				void transferOwnership(VulkanContext& vulkanCtx, uint32_t oldQueueIdx, uint32_t newQueueIdx);

				uint32_t getAlignedOffset(uint32_t bytes, uint32_t alignTo);

				inline uint32_t getMainBuffOwner() {
					return mMainBuffer.getQueueOwner();
				}

				void beginBatchUpload();

				void endBatchUpload();

				void mainBuffAcquireQueueOwnership(Wrapper::CommandBuffer& commandBuffer, uint32_t commandBufferIdx, uint32_t oldOwnerIdx, uint32_t newOwnerIdx, vk::raii::Queue& queue, vk::raii::Fence& fence);

				inline vk::Buffer getMainBuffer() {
					return mMainBuffer.getBuffer();
				}

				void bindImage(vk::raii::Image const& img, Memory const& memory);
				

				void transferMemoryToLocalBuffer(VulkanContext& vulkanCtx, Memory const &memory, std::size_t dataSize, bool endOfBatch = true);

				template<typename T>
				Memory allocateMemory(uint32_t size) {


					if (mMemoryTracker.size() == 0) {
						mMemoryTracker.push_back(Memory{ 0, size, false });
						mCurrentMainBufferSize -= size;
						return mMemoryTracker.back();

					}

					Memory backIter{ mMemoryTracker.back() };


					mCurrentMainBufferSize -= size;

					assert(mCurrentMainBufferSize > 0 && "WARNING, OUT OF MEMORY!");
					mMemoryTracker.push_back(Memory{ getAlignedOffset(backIter.getOffset() + backIter.getSize(), alignof(T)), size, false });
					return mMemoryTracker.back();

				}


				template<typename T>
				Memory allocateDescriptorMemory(vk::raii::PhysicalDevice& device, uint32_t size) {

					

					if (mDescriptorMemoryTracker.size() == 0) {
						mDescriptorMemoryTracker.push_back(Memory{ 0, size, false });
						return mDescriptorMemoryTracker.back();

					}

					Memory backIter{ mDescriptorMemoryTracker.back() };

					//assert(mCurrentMainBufferSize > 0 && "WARNING, OUT OF MEMORY!");
					mDescriptorMemoryTracker.push_back(Memory{ getAlignedOffset(backIter.getOffset() + backIter.getSize(), device.getProperties().limits.minUniformBufferOffsetAlignment), size, false });

					return mDescriptorMemoryTracker.back();

				}

				Memory allocateImageMemory(uint32_t size, uint32_t alignment) {

					if (mImageMemoryTracker.size() == 0) {
						mImageMemoryTracker.push_back(Memory{ 0, size, false });
						return mImageMemoryTracker.back();

					}

					Memory backIter{ mImageMemoryTracker.back() };

					mImageMemoryTracker.push_back(Memory{ getAlignedOffset(backIter.getOffset() + backIter.getSize(), alignment), size, false });
					return mImageMemoryTracker.back();

				}

				template<typename Iterable>
				void addToMappedMemory(Iterable& container, uint32_t offset) {
					
					uint8_t* cvtBytePointer = dynamic_cast<uint8_t*>(mDescriptorMappedBuff);

					memcpy(cvtBytePointer + offset, container.data(), container.size() * sizeof(*container.begin()));
				}

				template<typename Iterable>
				void addToDeviceBuffer(Iterable& container, VulkanContext &vulkanContext) {
					vulkanContext.device->waitForFences(*mTransferFence, vk::True, UINT64_MAX);
					vulkanContext.device->resetFences(*mTransferFence);

					//probably need to handle a case to do this in batches, cuz mStagingBuffer is less than the MainBuffer and what if container exceeds this (hypothetical)
					memcpy(mMappedStagingBuff, container.data(), container.size() * sizeof(*container.begin()));
				}

				DeviceMemoryManager(VulkanContext& vulkanCtx);

				~DeviceMemoryManager();
		};

	}

}

#endif // DEVICEMEMMANAGER_HPP