#ifndef CELERENGINE_HPP
#define CELERENGINE_HPP

#include "vulkanInstance.hpp"
#include "window.hpp"
#include "contexts.hpp"
#include "swapchain.hpp"
#include "pipelineBuilder.hpp"
#include "pipeline.hpp"
#include "commandBuffer.hpp"
#include "renderer.hpp"
#include "deviceMemManager.hpp"
#include "mesh.hpp"

namespace Celer {

	namespace Core {
		
		class CelerEngine {
			
			private:

				/*Contexts*/

				VulkanContext mVulkanContext;
				SwapchainContext mSwapChainContext;

				/*Private Objects*/

				Window mWindow;
				VulkanInstance mVulkanInstance;
				Swapchain mSwapchain;
				Render::Pipeline mPipeline; /*remove later pls*/
				//Wrapper::CommandBuffer mCommandBuffer;
				Core::DeviceMemoryManager mDeviceMemManager;

				Render::Renderer mRenderer;

				Geometry::MeshManager mMeshManager;

				/*Private Functions*/

			public:
				CelerEngine();
				~CelerEngine();

				void run();

		};







	}

}






#endif // CELERENGINE_HPP