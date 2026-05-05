#ifndef CELERENGINE_HPP
#define CELERENGINE_HPP

#include "vulkanInstance.hpp"
#include "window.hpp"
#include "contexts.hpp"
#include "swapchain.hpp"
#include "pipelineBuilder.hpp"
#include "pipeline.hpp"


namespace Celer {

	namespace Core {
		
		class CelerEngine {
			
			private:

				/*Private Objects*/

				Window mWindow;
				VulkanInstance mVulkanInstance;
				Swapchain mSwapchain;
				Pipeline mPipeline; /*remove later pls*/

				/*Contexts*/

				VulkanContext mVulkanContext;
				SwapchainContext mSwapChainContext;


				/*Private Functions*/


				


			public:
				CelerEngine();
				void run();

		};







	}

}






#endif // CELERENGINE_HPP