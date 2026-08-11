#ifndef RENDERER_HPP
#define RENDERER_HPP

#include "contexts.hpp"
#include "commandBuffer.hpp"
#include "swapchain.hpp"
#include "mesh.hpp"
#include "pipeline.hpp"
#include "sync.hpp"

namespace Celer {

	namespace Render {
		
		class Renderer {
			private:
				uint32_t mCurrentFrameIdx{};



				void recordDrawCommands(uint32_t imageIdx, Core::SwapchainContext& swapchainCtx, Pipeline& pipeline, Geometry::MeshManager &meshManager, Core::Window &window);


				Wrapper::CommandBuffer mCommandBuffer;

				vk::raii::Fence mTransferFence{ nullptr };
				std::vector<vk::raii::Fence> mFenceCollection;
				std::vector<vk::raii::Semaphore> mRenderFinished;
				std::vector<vk::raii::Semaphore> mPresentFinished;

				std::array<uint64_t, 2> test{ 0 };

			public:
				Renderer() = default;

				void drawFrame(Core::VulkanContext& vulkanCtx, Core::SwapchainContext& swapchainCtx, Pipeline& pipeline, Core::Swapchain& swapchain, Core::Window& window, Geometry::MeshManager& meshManager, Core::DeviceMemoryManager& memManager, Core::FrameContext& frameCtx);
				void init(Core::SwapchainContext& swapchainCtx, Core::VulkanContext& vulkanCtx);
				static constexpr uint32_t MAX_FRAMES_IN_FLIGHT{ 2 };

		};
	}

}

#endif // RENDERER_HPP