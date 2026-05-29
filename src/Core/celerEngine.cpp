#include "celerEngine.hpp"
#include <algorithm>
#include <vector>
#include <stdexcept>
#include "vulkanBuffer.hpp"

namespace Celer {

	namespace Core {

		CelerEngine::CelerEngine() : 
			mWindow{ "Vulkan", 1000, 800 }, 
			mVulkanInstance( mWindow, mVulkanContext ), 
			mSwapchain(mVulkanContext, mSwapChainContext, mWindow),
			mDeviceMemManager(mVulkanContext)
		
		{

			mWindow.setResizeEvent();

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

			Wrapper::Buffer buffer(1024 * 1024 * 500, vk::BufferUsageFlagBits::eVertexBuffer, vk::MemoryPropertyFlagBits::eHostCoherent | vk::MemoryPropertyFlagBits::eHostVisible, mVulkanContext);
			
			while (!mWindow.shouldClose()) {
				mWindow.pollEvents();

				mWindow.setWindowUserPointer();

				//if (mWindow.getResized()) std::cout << "RESIZED\n";

				mRenderer.drawFrame(mVulkanContext, mSwapChainContext, mPipeline.getPipeline(), mSwapchain, mWindow);
				
			}

		}



		






	}

}
