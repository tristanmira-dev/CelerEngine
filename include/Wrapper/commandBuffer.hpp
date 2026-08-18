#ifndef COMMANDBUFFER_HPP
#define COMMANDBUFFER_HPP

namespace Celer {

	namespace Wrapper {
		
		class CommandBuffer {
			private:

				vk::raii::CommandPool mCommandPool{ nullptr };
				vk::raii::CommandBuffers mCommandBuffers{ nullptr };

				
			public:

				void resetSingleBuff();
				
				CommandBuffer() = default;
				CommandBuffer(vk::raii::Device& device, uint32_t count, uint32_t queueFamilyIdx, vk::CommandBufferLevel bufferLevel = vk::CommandBufferLevel::ePrimary);

				inline vk::raii::CommandBuffer& getCommandBuffer(uint32_t idx) {
					return mCommandBuffers[idx];
				}

				void beginSingleTimeCommand();

				vk::raii::CommandBuffer& getSingleBuffer();

				void endSyncCommand(vk::raii::Queue& queue, uint32_t commandBufferIdx, vk::raii::Semaphore* signalSemaphore = nullptr, vk::raii::Semaphore* waitSemaphore = nullptr, vk::PipelineStageFlags pipelineStage = vk::PipelineStageFlagBits::eTopOfPipe, vk::raii::Fence *fence = nullptr);
				void endSingleTimeCommand(vk::raii::Queue& queue, vk::raii::Semaphore* semaphore = nullptr);


		};
	
	}

}

#endif // COMMANDBUFFER_HPP