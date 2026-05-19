#ifndef VULKANINSTANCE_HPP
#define VULKANINSTANCE_HPP

#include "contexts.hpp"
#include "window.hpp"

namespace Celer {

	namespace Core {

		class VulkanInstance {

			private:

				/*DEBUG VARS--------------------*/

				#if NDEBUG
					const bool mEnableValidationLayers{ false };
				#else
					const bool mEnableValidationLayers{ true };
				#endif

				const std::vector<char const*> mValidationLayers {
					"VK_LAYER_KHRONOS_validation"
				};

				/*Vulkan Objects--------------------*/

				vk::raii::Context mContext;
				vk::raii::Instance mInstance{ nullptr };
				vk::raii::DebugUtilsMessengerEXT mDebugMessenger{ nullptr };
				vk::raii::SurfaceKHR mSurface{ nullptr };
				vk::raii::PhysicalDevice mPhysicalDevice{ nullptr };
				vk::raii::Device mDevice{ nullptr };
				vk::raii::Queue mGraphicsQueue{ nullptr };
				vk::raii::Queue mPresentQueue{ nullptr };

				/*Objects*/
				std::vector<const char*> mDeviceExtensions = {
					vk::KHRSwapchainExtensionName /*extension of the gpu for presenting to the monitor*/
				};

				uint32_t mGraphicsQueueIdx{ static_cast<uint32_t>(~0) };
				uint32_t mPresentQueueIdx{ static_cast<uint32_t>(~0) };



				/*Private Functs--------------------*/
				
				/*
					init the vulkan instance raii object
				*/
				void initInstance();


				/*
					validation layers - init the debug messenger
				*/
				void setupDebugMessenger();



			public:


				VulkanInstance(Window& window, VulkanContext& vulkanContext);
				~VulkanInstance() = default;
				
				/*
					used to create a bridge between the window manager and vulkan, the presentation engine takes the finished image from the swapchain and presents it here
				*/
				void createSurface(VkSurfaceKHR surface);

				VulkanContext getVulkanContext();

				void pickPhysicalDevice();

				void createLogicalDevice();

				inline vk::raii::Instance& getInstance() {
					return mInstance;
				}

				void init(vk::SurfaceKHR& surface);




		};







	}

}

#endif // VULKANINSTANCE_HPP