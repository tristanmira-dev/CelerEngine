#include "celerEngine.hpp"
#include <algorithm>
#include <vector>
#include <stdexcept>

namespace Celer {

	namespace Core {

		CelerEngine::CelerEngine() : window{ "Vulkan", 1000, 800 }, vulkanInstance{}, vulkanContext{ vulkanInstance.getVulkanContext() } {

			/*Vulkan Instance Init*/
			vulkanInstance.createSurface(window.createSurface(*vulkanContext.instance));
			vulkanInstance.pickPhysicalDevice();
			vulkanInstance.createLogicalDevice();

			/*Swapchain Init*/
			swapchain.createSwapchain(*vulkanContext.device, *vulkanContext.physicalDevice, *vulkanContext.surface, window.getWindow());

		}


		void CelerEngine::run() {

			while (!window.shouldClose()) {
				glfwPollEvents();
			}

		}



		






	}

}
