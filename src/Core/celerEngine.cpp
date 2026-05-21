#include "celerEngine.hpp"
#include <algorithm>
#include <vector>
#include <stdexcept>

namespace Celer {

	namespace Core {

		CelerEngine::CelerEngine() : 
			mWindow{ "Vulkan", 1000, 800 }, 
			mVulkanInstance( mWindow, mVulkanContext ), 
			mSwapchain(mVulkanContext, mSwapChainContext, mWindow) 
		
		{

			/*Pipeline*/
			Render::PipelineBuilder pipelineBuilder;

			pipelineBuilder.createShader(
				"./assets/shaders/slang.spv",
				*mVulkanContext.device
			);

			pipelineBuilder.addShaderStage(vk::ShaderStageFlagBits::eVertex, "vertMain");
			pipelineBuilder.addShaderStage(vk::ShaderStageFlagBits::eFragment, "fragMain");

			

			/*TEST, JUST PUT PIPELINE INTO A CONTAINER LATER OR SOMETHING---------------------------------------------------------------------------------------------->*/
			mPipeline.setPipeline(pipelineBuilder, *mVulkanContext.device, *mSwapChainContext.swapchainSurfaceFormat);
			
			mRenderer.init(mSwapChainContext, mVulkanContext);

		}

		CelerEngine::~CelerEngine() {
			mVulkanContext.device->waitIdle();
		}


		void CelerEngine::run() {

			while (!mWindow.shouldClose()) {
				mWindow.pollEvents();

				mRenderer.drawFrame(mVulkanContext, mSwapChainContext, mPipeline.getPipeline());
				
			}

		}



		






	}

}
