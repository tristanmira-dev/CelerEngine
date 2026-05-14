#ifndef SWAPCHAIN_HPP
#define SWAPCHAIN_HPP

#include "contexts.hpp"
#include "image.hpp"

namespace Celer {
	namespace Core {
		class Swapchain {
			
			private:
				/*Objects*/

				uint32_t mSwapchainImagesCount;

				/*Vulkan objects*/

				vk::raii::SwapchainKHR mSwapchain{ nullptr };
				vk::SurfaceFormatKHR mSwapchainSurfaceFormat;
				Wrapper::NonOwnedImageCollection mSwapchainImages;
				vk::Extent2D mSwapchainExtent;

				/*Functions*/

				/*
				*	Return SurfaceFormatKHR based on vk::Format::eB8G8R8A8Srgb && vk::ColorSpaceKHR::eSrgbNonlinear. Ensures that the memory layout is 32-bit BGRA with hardware sRGB encoding, and the presentation pipeline interprets and displays it as sRGB
				*/
				vk::SurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<vk::SurfaceFormatKHR>& availableFormats);

				/*
				*	Return PresentModeKHR based on the physical device specs
				*/
				vk::PresentModeKHR chooseSwapPresentMode(const std::vector<vk::PresentModeKHR>& availPresentModes);

				/*
				*	Return Extent2D based on the physical device specs
				*/
				vk::Extent2D chooseSwapExtent(vk::SurfaceCapabilitiesKHR const& capabilities, GLFWwindow* window);

				


			public:

				Swapchain(uint32_t swapchainImages = 3);
				~Swapchain() = default;

				void createSwapchain(vk::raii::Device &device, vk::raii::PhysicalDevice& physicalDevice, vk::raii::SurfaceKHR& surface, GLFWwindow* window, std::array<uint32_t, 2> queueIndices);
				void createImageView(vk::raii::Device &device);
				SwapchainContext getContext();
		};
	}
}




#endif // SWAPCHAIN_HPP