#ifndef RENDERER_HPP
#define RENDERER_HPP

#include "contexts.hpp"
#include "commandBuffer.hpp"

namespace Celer {

	namespace Render {
		
		class Renderer {
			private:
				uint32_t mCurrentFrameIdx{};
				void drawFrame(Core::VulkanContext& vulkanCtx, Core::SwapchainContext& swapchainCtx, vk::raii::Pipeline& pipeline);
				void recordDrawCommands(uint32_t imageIdx, Core::SwapchainContext& swapchainCtx, vk::raii::Pipeline& pipeline);


				Wrapper::CommandBuffer mCommandBuffer;
				std::vector<vk::raii::Fence> mFenceCollection;
				std::vector<vk::raii::Semaphore> mRenderFinished;
				std::vector<vk::raii::Semaphore> mPresentFinished;


			public:
				Renderer() = default;
				void init(Core::SwapchainContext& swapchainCtx, Core::VulkanContext& vulkanCtx);
				static constexpr uint32_t MAX_FRAMES_IN_FLIGHT{ 2 };

		};
	}

}

#endif // RENDERER_HPP