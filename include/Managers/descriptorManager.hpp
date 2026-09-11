#ifndef MANAGER_DESCRIPTOR_HPP
#define MANAGER_DESCRIPTOR_HPP

#include "deviceMemManager.hpp"


namespace Celer {

	namespace Managers {
		class Descriptors {
			public:

				Descriptors(vk::raii::PhysicalDevice& device, Core::DeviceMemoryManager& memManager);


				std::vector<Core::Memory> mGameObjectPropsMemory;
				std::vector<Core::Memory> mViewPropsMemory;				
				


		};

	}

}






#endif