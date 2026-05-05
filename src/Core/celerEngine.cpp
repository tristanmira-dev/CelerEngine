#include "celerEngine.hpp"
#include <algorithm>
#include <vector>
#include <stdexcept>

namespace Celer {

	namespace Core {

		CelerEngine::CelerEngine() : mWindow{ "Vulkan", 1000, 800 }, mVulkanInstance{}, mVulkanContext{ mVulkanInstance.getVulkanContext() } {

			/*Vulkan Instance Init*/
			mVulkanInstance.createSurface(mWindow.createSurface(*mVulkanContext.instance));
			mVulkanInstance.pickPhysicalDevice();
			mVulkanInstance.createLogicalDevice();

			/*Swapchain Init*/
			mSwapchain.createSwapchain(*mVulkanContext.device, *mVulkanContext.physicalDevice, *mVulkanContext.surface, mWindow.getWindow(), std::array<uint32_t, 2>{mVulkanContext.graphicsQueueIdx, mVulkanContext.presentQueueIdx});
			mSwapChainContext = mSwapchain.getContext();


			/*Pipeline*/
			std::vector<vk::PipelineShaderStageCreateInfo> shader = {
				vk::PipelineShaderStageCreateInfo{.stage = vk::ShaderStageFlagBits::eVertex, .pName = "vertMain" },
				vk::PipelineShaderStageCreateInfo{.stage = vk::ShaderStageFlagBits::eFragment, .pName = "fragMain" }
			};

			PipelineBuilder pipelineBuilder;
			pipelineBuilder.createShader(
				"./assets/shaders/slang.spv",
				shader,
				*mVulkanContext.device
			);

			mPipeline.setPipeline(pipelineBuilder, *mVulkanContext.device, *mSwapChainContext.swapchainSurfaceFormat);
			
			


		}


		void CelerEngine::run() {

			while (!mWindow.shouldClose()) {
				glfwPollEvents();
			}

		}



		






	}

}
