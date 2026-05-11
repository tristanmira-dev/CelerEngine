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
			mSwapChainContext = mSwapchain.getContext();


			/*Pipeline*/
			std::vector<vk::PipelineShaderStageCreateInfo> shader = {
				vk::PipelineShaderStageCreateInfo{.stage = vk::ShaderStageFlagBits::eVertex, .pName = "vertMain" },
				vk::PipelineShaderStageCreateInfo{.stage = vk::ShaderStageFlagBits::eFragment, .pName = "fragMain" }
			};

			Render::PipelineBuilder pipelineBuilder;
			pipelineBuilder.createShader(
				"./assets/shaders/slang.spv",
				shader,
				*mVulkanContext.device
			);

			/*UNIT TEST---------------------------------------------------------------------------------------------->*/
			mPipeline.setPipeline(pipelineBuilder, *mVulkanContext.device, *mSwapChainContext.swapchainSurfaceFormat);

			mCommandBuffer = Wrapper::CommandBuffer(*mVulkanContext.device, 1, mVulkanContext.graphicsQueueIdx);
			


		}


		void CelerEngine::run() {

			while (!mWindow.shouldClose()) {
				glfwPollEvents();
			}

		}



		






	}

}
