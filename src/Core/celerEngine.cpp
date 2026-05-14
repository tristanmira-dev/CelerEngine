#include "celerEngine.hpp"
#include <algorithm>
#include <vector>
#include <stdexcept>

namespace Celer {

	namespace Core {

		CelerEngine::CelerEngine() : mWindow{ "Vulkan", 1000, 800 }, mVulkanInstance{} {

			/*Vulkan Instance Init*/
			mVulkanInstance.createSurface(mWindow.createSurface(mVulkanInstance.getInstance()));
			mVulkanInstance.pickPhysicalDevice();	
			mVulkanInstance.createLogicalDevice();
			mVulkanContext = mVulkanInstance.getVulkanContext();


			/*Swapchain Init*/
			mSwapchain.createSwapchain(*mVulkanContext.device, *mVulkanContext.physicalDevice, *mVulkanContext.surface, mWindow.getWindow(), std::array<uint32_t, 2>{mVulkanContext.graphicsQueueIdx, mVulkanContext.presentQueueIdx});
			mSwapchain.createImageView(*mVulkanContext.device);
			mSwapChainContext = mSwapchain.getContext();



			/*Pipeline*/
			Render::PipelineBuilder pipelineBuilder;

			pipelineBuilder.createShader(
				"./assets/shaders/slang.spv",
				*mVulkanContext.device
			);

			pipelineBuilder.addShaderStage(vk::ShaderStageFlagBits::eVertex, "vertMain");
			pipelineBuilder.addShaderStage(vk::ShaderStageFlagBits::eFragment, "fragMain");

			

			/*UNIT TEST---------------------------------------------------------------------------------------------->*/
			mPipeline.setPipeline(pipelineBuilder, *mVulkanContext.device, *mSwapChainContext.swapchainSurfaceFormat);

			mCommandBuffer = Wrapper::CommandBuffer(*mVulkanContext.device, 1, mVulkanContext.graphicsQueueIdx);
			


		}


		void CelerEngine::run() {

			while (!mWindow.shouldClose()) {
				mWindow.pollEvents();
			}

		}



		






	}

}
