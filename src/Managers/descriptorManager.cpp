#include "descriptorManager.hpp"
#include "commonTypes.hpp"
#include "descriptors.hpp"

namespace Celer {

	namespace Managers {
		Descriptors::Descriptors(vk::raii::PhysicalDevice& device, Core::DeviceMemoryManager& memManager) : mGameObjectPropsMemory(2), mViewPropsMemory(2) {
			const uint32_t FRAMES_IN_FLIGHT = 2;

			mGameObjectPropsMemory[0] = memManager.allocateDescriptorMemory<Geometry::GameObjectProperties>(device,Core::KILO * Core::KILO * 50);
			mGameObjectPropsMemory[1] = memManager.allocateDescriptorMemory<Geometry::GameObjectProperties>(device,Core::KILO * Core::KILO * 50);

			mViewPropsMemory[0] = memManager.allocateDescriptorMemory<Geometry::ViewProperties>(device, sizeof(Geometry::ViewProperties));
			mViewPropsMemory[1] = memManager.allocateDescriptorMemory<Geometry::ViewProperties>(device, sizeof(Geometry::ViewProperties));


		}
	}

}