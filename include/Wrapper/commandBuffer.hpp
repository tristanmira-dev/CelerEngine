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
				CommandBuffer(vk::raii::Device& device, uint32_t count, uint32_t queueFamilyIdx, vk::CommandBufferLevel bufferLevel = vk::CommandBufferLevel::ePrimary);

				vk::raii::CommandBuffer& getCommandBuffer(uint32_t idx);

		};
	
	}

}

#endif // COMMANDBUFFER_HPP