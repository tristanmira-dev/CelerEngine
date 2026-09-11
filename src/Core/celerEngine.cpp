#include "celerEngine.hpp"
#include "mesh.hpp"
#include "descriptors.hpp"

namespace Celer {

	namespace Core {

		CelerEngine::CelerEngine() : 
			mWindow{ "Vulkan", 1000, 800 }, 
			mVulkanInstance( mWindow, mVulkanContext ), 
			mSwapchain(mVulkanContext, mSwapChainContext, mWindow),
			mDeviceMemManager(mVulkanContext),
			mUploadManager(mVulkanContext),
			mMeshManager(mDeviceMemManager),
			mTextureManager(mDeviceMemManager, mVulkanContext),
			mFrameCtx(mVulkanContext),
			mDescriptorManager(*mVulkanContext.physicalDevice, mDeviceMemManager)
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


			mTextureManager.addTexture("./assets/textures/viking_room.png", mVulkanContext, mUploadManager);
			mTextureManager.addTexture("./assets/textures/eddieblanket_edge.png", mVulkanContext, mUploadManager);
			

			
			mPipeline.updateDescriptorImage(*mVulkanContext.device, mTextureManager.getImageView(0), mTextureManager.mSampler, 0);
			mPipeline.updateDescriptorImage(*mVulkanContext.device, mTextureManager.getImageView(1), mTextureManager.mSampler, 1);

			mPipeline.updateBufferDescriptors<Geometry::GameObjectProperties>(*mVulkanContext.device, mDescriptorManager.mGameObjectPropsMemory, mDeviceMemManager.getDescriptorBuffer(), vk::DescriptorType::eStorageBuffer, 1);

			mPipeline.updateBufferDescriptors<Geometry::ViewProperties>(*mVulkanContext.device, mDescriptorManager.mViewPropsMemory, mDeviceMemManager.getDescriptorBuffer(), vk::DescriptorType::eUniformBuffer, 2);
			
			mMeshManager.addVertices({
				{ glm::vec3(-1.5f, -0.5f,  5.0f), glm::vec3(1.f, 0.f, 0.f), glm::vec2(0.f, 1.f) }, // Bottom-Left
				{ glm::vec3(-0.5f, -0.5f,  3.5f), glm::vec3(0.f, 1.f, 0.f), glm::vec2(1.f, 1.f) }, // Bottom-Right 
				{ glm::vec3(-0.5f,  0.5f,  3.5f), glm::vec3(0.f, 0.f, 1.f), glm::vec2(1.f, 0.f) }, // Top-Right
				{ glm::vec3(-1.5f,  0.5f,  5.0f), glm::vec3(1.f, 1.f, 0.f), glm::vec2(0.f, 0.f) }, // Top-Left
			});


			mMeshManager.submitMesh(mVulkanContext, false);

			mMeshManager.addIndices({
				0, 1, 2, 0, 2, 3
			});

			mMeshManager.submitIndices(mVulkanContext, false);

			mGameObjectManager.addGameObject({ .transform = glm::mat4(1.f), .textureIdx = 0 } , { .textureIndex = 0, .meshIndex = 0 });

			mDeviceMemManager.transferOwnership(mVulkanContext, mVulkanContext.transferQueueIdx, mVulkanContext.graphicsQueueIdx);


			while (!mWindow.shouldClose()) {
				mWindow.pollEvents();

				mWindow.setWindowUserPointer();

				//if (mWindow.getResized()) std::cout << "RESIZED\n";

				//check to see if any asset is added, so local count == global count

				mUploadManager.update(mFrameCtx, mVulkanContext, mDeviceMemManager);

				mRenderer.drawFrame(mVulkanContext, mSwapChainContext, mPipeline, mSwapchain, mWindow, mMeshManager, mDeviceMemManager, mFrameCtx, mPipeline.mDescriptorSets, mGameObjectManager, mDescriptorManager);
				
			}

		}



		






	}

}
