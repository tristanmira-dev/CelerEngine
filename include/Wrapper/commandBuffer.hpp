#ifndef COMMANDBUFFER_HPP
#define COMMANDBUFFER_HPP

namespace Celer {

	namespace Wrapper {
		
		class CommandBuffer {
			private:

				vk::raii::CommandPool mCommandPool{ nullptr };
				vk::raii::CommandBuffers mCommandBuffers{ nullptr };

				
			public:

				CommandBuffer() = default;
				~CommandBuffer() = default;

				/*Move Constructor && Move assignment*/
				CommandBuffer(CommandBuffer&& commandBuffer) noexcept;
				CommandBuffer& operator=(CommandBuffer&& commandBuffer) noexcept;
				//

				/*Parametarized constructor*/
				CommandBuffer(vk::raii::Device& device, uint32_t count, uint32_t queueFamilyIdx, vk::CommandBufferLevel bufferLevel = vk::CommandBufferLevel::ePrimary);

				vk::raii::CommandBuffer& getCommandBuffer(uint32_t idx);

				vk::raii::CommandBuffer& operator[](std::size_t index);

				void resetSingleBuff();

				void beginSingleTimeCommand();

				vk::raii::CommandBuffer& getSingleBuffer();

				void endSyncCommand(vk::raii::Queue& queue, uint32_t commandBufferIdx, vk::raii::Semaphore* signalSemaphore = nullptr, vk::raii::Semaphore* waitSemaphore = nullptr, vk::PipelineStageFlags pipelineStage = vk::PipelineStageFlagBits::eTopOfPipe, vk::raii::Fence *fence = nullptr);
				void endSingleTimeCommand(vk::raii::Queue& queue, vk::raii::Semaphore* semaphore = nullptr);


		};
	
	}

}

#endif // COMMANDBUFFER_HPP