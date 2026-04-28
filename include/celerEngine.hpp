#ifndef CELERENGINE_HPP
#define CELERENGINE_HPP

#include "vulkanInstance.hpp"
#include "window.hpp"
#include "contexts.hpp"
#include "swapchain.hpp"


namespace Celer {

	namespace Core {
		
		class CelerEngine {
			
			private:

				/*Private Objects*/

				Window window;
				VulkanInstance vulkanInstance;
				Swapchain swapchain;

				/*Contexts*/

				VulkanContext vulkanContext;

				/*Private Functions*/

			public:
				CelerEngine();
				void run();

		};







	}

}






#endif // CELERENGINE_HPP