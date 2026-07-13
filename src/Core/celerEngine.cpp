#include "celerEngine.hpp"
#include "mesh.hpp"

namespace Celer {

	namespace Core {

		CelerEngine::CelerEngine() : 
			mWindow{ "Vulkan", 1000, 800 }, 
			mVulkanInstance( mWindow, mVulkanContext ), 
			mSwapchain(mVulkanContext, mSwapChainContext, mWindow),
			mDeviceMemManager(mVulkanContext),
			mMeshManager(mDeviceMemManager),
			mTextureManager(mDeviceMemManager)
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

			//Wrapper::Buffer buffer(1024 * 1024 * 500, vk::BufferUsageFlagBits::eVertexBuffer, vk::MemoryPropertyFlagBits::eHostCoherent | vk::MemoryPropertyFlagBits::eHostVisible, mVulkanContext);
			//

			//float2(0.0, -0.5),
			//	float2(0.5, 0.5),
			//	float2(-0.5, 0.5)
			mMeshManager.addVertices({
				{ glm::vec3(-1.5f, -0.5f,  5.f), glm::vec3(1.f, 0.f, 0.f) },
				{ glm::vec3(-0.5f, -0.5f,  5.f), glm::vec3(0.f, 1.f, 0.f) },
				{ glm::vec3(-0.5f,  0.5f,  5.f), glm::vec3(0.f, 0.f, 1.f) },
				{ glm::vec3(-1.5f,  0.5f,  5.f), glm::vec3(1.f, 1.f, 0.f) },
				{ glm::vec3(-1.5f, -0.5f,  4.5f), glm::vec3(1.f, 0.f, 1.f) },
				{ glm::vec3(-0.5f, -0.5f,  4.5f), glm::vec3(0.f, 1.f, 1.f) },
				{ glm::vec3(-0.5f,  0.5f,  4.5f), glm::vec3(1.f, 1.f, 1.f) },
				{ glm::vec3(-1.5f,  0.5f,  4.5f), glm::vec3(0.f, 0.f, 0.f) },
			});


			mMeshManager.submitMesh(mVulkanContext, false);

			mMeshManager.addIndices({
				0, 1, 2,  0, 2, 3, 
				1, 5, 6,  1, 6, 2, 
				5, 4, 7,  5, 7, 6, 
				4, 0, 3,  4, 3, 7, 
				3, 2, 6,  3, 6, 7, 
				4, 5, 1,  4, 1, 0, 
			});

			mMeshManager.submitIndices(mVulkanContext, false);



			mDeviceMemManager.transferOwnership(mVulkanContext, mVulkanContext.transferQueueIdx, mVulkanContext.graphicsQueueIdx);


			while (!mWindow.shouldClose()) {
				mWindow.pollEvents();

				mWindow.setWindowUserPointer();

				//if (mWindow.getResized()) std::cout << "RESIZED\n";

				mRenderer.drawFrame(mVulkanContext, mSwapChainContext, mPipeline, mSwapchain, mWindow, mMeshManager, mDeviceMemManager);
				
			}

		}



		






	}

}
