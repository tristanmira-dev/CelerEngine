#ifndef CONTEXTS_HPP
#define CONTEXTS_HPP

namespace Celer {
	
	namespace Core {
		struct VulkanContext {
			vk::raii::Context* context;
			vk::raii::Instance* instance;
			vk::raii::PhysicalDevice* physicalDevice;
			vk::raii::SurfaceKHR* surface;
			vk::raii::Device* device;
			vk::raii::Queue* graphicsQueue;
			vk::raii::Queue* presentQueue;

			uint32_t graphicsQueueIdx;
			uint32_t presentQueueIdx;
		};
	}

}


#endif // CONTEXTS_HPP