#ifndef CONTEXTS_HPP
#define CONTEXTS_HPP

#include "image.hpp"

namespace Celer {
	
	namespace Core {
		struct VulkanContext {
			vk::raii::Context* context{};
			vk::raii::Instance* instance{};
			vk::raii::PhysicalDevice* physicalDevice{};
			vk::raii::SurfaceKHR* surface{};
			vk::raii::Device* device{};
			vk::raii::Queue* graphicsQueue{};
			vk::raii::Queue* presentQueue{};
			vk::raii::Queue* transferQueue{};

			uint32_t graphicsQueueIdx = ~0;
			uint32_t presentQueueIdx = ~0;
			uint32_t transferQueueIdx = ~0;
		};


		struct SwapchainContext {
			//std::vector<vk::Image>* swapchainImages;
			vk::SurfaceFormatKHR* swapchainSurfaceFormat;
			//std::vector<vk::ImageView>* swapchainImageViews;
			vk::Extent2D* swapchainExtent;
			
			Wrapper::NonOwnedImageCollection* swapChainImages;

			vk::raii::SwapchainKHR* swapchain;
		};
	}

}


#endif // CONTEXTS_HPP