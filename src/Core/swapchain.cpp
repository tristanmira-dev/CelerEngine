#include "swapchain.hpp"

namespace Celer {
	namespace Core {
		
		Swapchain::Swapchain(VulkanContext& vulkanContext, SwapchainContext& swapchainContext, Window& mWindow, uint32_t swapchainImages) : mSwapchainImagesCount{ swapchainImages } {
		
			createSwapchain(*vulkanContext.device, *vulkanContext.physicalDevice, *vulkanContext.surface, mWindow, { vulkanContext.graphicsQueueIdx, vulkanContext.presentQueueIdx });
			createImageView(*vulkanContext.device);
			swapchainContext = getContext();
			
		}

		vk::SurfaceFormatKHR Swapchain::chooseSwapSurfaceFormat(const std::vector<vk::SurfaceFormatKHR>& availableFormats) {

			for (vk::SurfaceFormatKHR const& availableFormat : availableFormats) {
				if (availableFormat.format == vk::Format::eB8G8R8A8Srgb /*actual format in memory*/ && availableFormat.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear /*Interpreter*/) {
					return availableFormat;
				}
			}

			return availableFormats[0];
		}

		vk::PresentModeKHR Swapchain::chooseSwapPresentMode(const std::vector<vk::PresentModeKHR>& availPresentModes) {

			for (vk::PresentModeKHR const& present : availPresentModes) {
				if (present == vk::PresentModeKHR::eMailbox) return present;
			}
			return vk::PresentModeKHR::eFifo;
		}

		vk::Extent2D Swapchain::chooseSwapExtent(vk::SurfaceCapabilitiesKHR const& capabilities, Window& window) {

			/*Just go along with vulkan's default if condition is met (currentExtent hits the max), aka ur window giving you a default extent*/
			if (capabilities.currentExtent.width != (std::numeric_limits<uint32_t>::max)() /*Weird, getting shadowed by another max macro*/) {
				return capabilities.currentExtent;
			}
			int width, height;
			window.getFrameBufferSize(width, height);

			/*Just choose the resolution of the window, taking into account the surface's extents*/
			return vk::Extent2D{
				std::clamp<uint32_t>(width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width),
				std::clamp<uint32_t>(height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height)
			};
		}

		void Swapchain::cleanup() {
			mSwapchainImages.clear();
			mSwapchain = nullptr;
		
		}

		SwapchainContext Swapchain::getContext() {
			return SwapchainContext{
				//.swapchainImages = &mSwapchainImages,
				.swapchainSurfaceFormat = &mSwapchainSurfaceFormat,
				//.swapchainImageViews = &mSwapchainImageViews,
				.swapchainExtent = &mSwapchainExtent,

				.swapChainImages = &mSwapchainImages,

				.swapchain = &mSwapchain

			};
		}

		void Swapchain::recreateSwapchain(VulkanContext &vulkanContext, Window & mWindow) {
			vulkanContext.device->waitIdle();
			cleanup();
			createSwapchain(*vulkanContext.device, *vulkanContext.physicalDevice, *vulkanContext.surface, mWindow, { vulkanContext.graphicsQueueIdx, vulkanContext.presentQueueIdx });
			createImageView(*vulkanContext.device);
		
		}

		void Swapchain::createSwapchain(vk::raii::Device &device, vk::raii::PhysicalDevice &physicalDevice, vk::raii::SurfaceKHR &surface, Window &window, std::array<uint32_t,2> queueIndices) {
			
			auto surfaceCapabilities{ physicalDevice.getSurfaceCapabilitiesKHR(*surface) };
			mSwapchainSurfaceFormat = chooseSwapSurfaceFormat(physicalDevice.getSurfaceFormatsKHR(*surface));
			mSwapchainExtent = chooseSwapExtent(surfaceCapabilities, window);

			auto minImageCount{ std::max(mSwapchainImagesCount, surfaceCapabilities.minImageCount) };
			minImageCount = (surfaceCapabilities.maxImageCount > 0 && minImageCount > surfaceCapabilities.maxImageCount) ? surfaceCapabilities.maxImageCount : minImageCount; /*maxImageCount can be 0 indicating unlimited*/

			/*Create Props struct and swapchain here....*/
			vk::SwapchainCreateInfoKHR swapChainCreateInfo{
				.flags = vk::SwapchainCreateFlagBitsKHR(),
				.surface = *surface,
				.minImageCount = minImageCount,
				.imageFormat = mSwapchainSurfaceFormat.format,
				.imageExtent = mSwapchainExtent,
				.imageArrayLayers = 1,
				.imageUsage = vk::ImageUsageFlagBits::eColorAttachment, 
				.preTransform = surfaceCapabilities.currentTransform,
				.compositeAlpha = vk::CompositeAlphaFlagBitsKHR::eOpaque,
				.presentMode = chooseSwapPresentMode(physicalDevice.getSurfacePresentModesKHR(*surface)),
				.clipped = true,
				.oldSwapchain = nullptr
			};

			if (queueIndices[0] == queueIndices[1]) swapChainCreateInfo.imageSharingMode = vk::SharingMode::eExclusive;
			else {
				swapChainCreateInfo.imageSharingMode = vk::SharingMode::eConcurrent;
				swapChainCreateInfo.pQueueFamilyIndices = queueIndices.data();
				swapChainCreateInfo.queueFamilyIndexCount = 2;
			}

			mSwapchain = vk::raii::SwapchainKHR(device, swapChainCreateInfo);

			std::cout << "Swapchain images: " << mSwapchain.getImages().size() /*3 images In this case (default value) */ << "\n";
			std::cout << "Format: " << vk::to_string(mSwapchainSurfaceFormat.format) << "\n";
			std::cout << "Extent: " << mSwapchainExtent.width << "x" << mSwapchainExtent.height << "\n";
			std::cout << physicalDevice.getProperties().deviceName << '\n';

		}


		void Swapchain::createImageView(vk::raii::Device &device) {

			mSwapchainImages.addImagesWithView(mSwapchain.getImages(), mSwapchainSurfaceFormat.format, device);
			std::cout << "Swapchain image (handle to non-owned) and imageviews created!\n";
		
		}

	}
}