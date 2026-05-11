#include "vulkanInstance.hpp"
#include "extensions.hpp"

#include <set>

namespace {
	/*
		called by validation layers (VK_EXT_debug_utils)
	*/
	VKAPI_ATTR vk::Bool32 VKAPI_CALL debugCallback(vk::DebugUtilsMessageSeverityFlagBitsEXT severity,
		vk::DebugUtilsMessageTypeFlagsEXT type,
		const vk::DebugUtilsMessengerCallbackDataEXT* pCallbackData,
		void* pUserData
	) {

		std::cerr << "validation layer: type " << to_string(type) << " msg: " << pCallbackData->pMessage << std::endl;

		return vk::False;
	}


}

namespace Celer {

	namespace Core {
		
		void VulkanInstance::initInstance() {
			std::vector<char const*> requiredLayers;
			if (mEnableValidationLayers) requiredLayers.assign(mValidationLayers.begin(), mValidationLayers.end());

			auto layerProps{ mContext.enumerateInstanceLayerProperties() };

			/*Check if layer is supported via brute-force*/
			auto layerIT{
				std::ranges::find_if(requiredLayers, [&layerProps](const char* const& constlayer) {
					return std::ranges::none_of(layerProps, [&constlayer](vk::LayerProperties const& props) {
						return std::strcmp(constlayer, props.layerName) == 0;
					});
				})
			};

			if (layerIT != requiredLayers.end()) {
				throw std::runtime_error{ *layerIT };
			}

			constexpr vk::ApplicationInfo appInfo{
				.pApplicationName = "Tri", .applicationVersion = VK_MAKE_VERSION(1,0,0), .pEngineName = "No Engine", .engineVersion = VK_MAKE_VERSION(1,0,0), .apiVersion = vk::ApiVersion14
			};

			/*Mostly glfw extensions to compare against vulkan supported extensions, also validation layer extension*/
			std::vector<const char*> requiredExtensions{ getRequiredInstanceExtensions(mEnableValidationLayers) };

			/*Get current extensions supported by vulkan*/
			auto extensionProps{ mContext.enumerateInstanceExtensionProperties() };

			/*Check if an extension is supported via brute-force*/
			auto requiredExtIT{
				std::ranges::find_if(requiredExtensions, [&extensionProps](const char* const& required) {
					return std::ranges::none_of(extensionProps, [&required](vk::ExtensionProperties const& extProps) {
						return std::strcmp(required, extProps.extensionName) == 0;
					});
				})
			};

			if (requiredExtIT != requiredExtensions.end()) {
				throw std::runtime_error{ *requiredExtIT };
			}

			vk::InstanceCreateInfo createInfo{
				.pApplicationInfo = &appInfo,
				.enabledLayerCount = static_cast<uint32_t>(requiredLayers.size()),
				.ppEnabledLayerNames = requiredLayers.data(),
				.enabledExtensionCount = static_cast<uint32_t>(requiredExtensions.size()),
				.ppEnabledExtensionNames = requiredExtensions.data()
			};

			mInstance = vk::raii::Instance{ mContext, createInfo };
		}

		void VulkanInstance::setupDebugMessenger() {
			if (!mEnableValidationLayers) { return; }

			vk::DebugUtilsMessageSeverityFlagsEXT severityFlags{
				vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose |
				vk::DebugUtilsMessageSeverityFlagBitsEXT::eInfo |
				vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning |
				vk::DebugUtilsMessageSeverityFlagBitsEXT::eError
			};

			vk::DebugUtilsMessageTypeFlagsEXT messageTypeFlags{
				vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral |
				vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance |
				vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation
			};

			vk::DebugUtilsMessengerCreateInfoEXT debugUtilsMessengerCreateInfoEXT{
				.messageSeverity = severityFlags,
				.messageType = messageTypeFlags,
				.pfnUserCallback = debugCallback
			};
			mDebugMessenger = mInstance.createDebugUtilsMessengerEXT(debugUtilsMessengerCreateInfoEXT);
		}

		void VulkanInstance::pickPhysicalDevice() {
			auto devices{ mInstance.enumeratePhysicalDevices() }; /*Get physical devices*/
			const auto devicesIter{

				std::ranges::find_if(devices, [&](vk::raii::PhysicalDevice const& device) {
					auto queueFamilies{device.getQueueFamilyProperties()};
					bool isSuitable{ device.getProperties().apiVersion >= VK_API_VERSION_1_3 };

					/*In a gpu, there are different 'families' called queue families that deal with different tasks. think of it like individual factories that are in charge of producing different items*/
					const auto qfpIter{
						std::ranges::find_if(queueFamilies, [](vk::QueueFamilyProperties const& queueProps) {
							return (queueProps.queueFlags & vk::QueueFlagBits::eGraphics) != static_cast<vk::QueueFlags>(0); /*just to make sure the graphics part of the family is supported*/
						})
					};

					isSuitable = isSuitable && (qfpIter != queueFamilies.end());

					auto extensions = device.enumerateDeviceExtensionProperties();
					bool found{ true };

					/*Just seeing if the current physical device in the iteration can accomodate presenting on the monitor*/
					for (char const* const& extension : mDeviceExtensions) {
						auto extensionIter{ std::ranges::find_if(extensions, [extension](vk::ExtensionProperties const& props) {
							return std::strcmp(extension, props.extensionName) == 0;
						})};

						found = found && extensionIter != extensions.end();
					}

					isSuitable = found && isSuitable;

					return isSuitable;
				})

			};

			if (devices.empty() || (devicesIter == devices.end())) { throw std::runtime_error{ "Mate you don't even have a proper GPU" }; }
			mPhysicalDevice = *devicesIter;
		}

		void VulkanInstance::createLogicalDevice() {
			std::vector<vk::QueueFamilyProperties> queueFamilyProperties = mPhysicalDevice.getQueueFamilyProperties();
			auto graphicsQueueFamilyProperty{ std::ranges::find_if(queueFamilyProperties, [](const vk::QueueFamilyProperties& props) {
				return (props.queueFlags & vk::QueueFlagBits::eGraphics) != static_cast<vk::QueueFlags>(0);
			}) };

			uint32_t graphicsIdx{ static_cast<uint32_t>(std::distance(queueFamilyProperties.begin(), graphicsQueueFamilyProperty)) };

			auto presentIndex{ mPhysicalDevice.getSurfaceSupportKHR(graphicsIdx, *mSurface) ? graphicsIdx : static_cast<uint32_t>(queueFamilyProperties.size()) };

			/*if index(currentFamily that supports graphics) that was found using the find_if cannot support the current surface*/
			if (presentIndex == queueFamilyProperties.size()) {

				/*Search for families that support both present(surface) and graphics*/
				for (std::size_t i{}; i < queueFamilyProperties.size(); ++i) {
					if ((queueFamilyProperties[i].queueFlags & vk::QueueFlagBits::eGraphics) && mPhysicalDevice.getSurfaceSupportKHR(i, *mSurface)) {
						graphicsIdx = static_cast<uint32_t>(i);
						presentIndex = static_cast<uint32_t>(i);
						break;
					}
				}

				/*Search for a different family that supports surface*/
				if (presentIndex == queueFamilyProperties.size()) {
					for (std::size_t i{}; i < queueFamilyProperties.size(); ++i) {
						if (mPhysicalDevice.getSurfaceSupportKHR(static_cast<uint32_t>(i), *mSurface)) {
							presentIndex = static_cast<uint32_t>(i);
							break;
						}
					}
				}
			}


			if ((graphicsIdx == static_cast<uint32_t>(queueFamilyProperties.size())) || (presentIndex == static_cast<uint32_t>(queueFamilyProperties.size()))) {
				throw std::runtime_error{ "No appropriate graphics or surface compatible queue found" };
			}


			vk::StructureChain<vk::PhysicalDeviceFeatures2, vk::PhysicalDeviceVulkan11Features, vk::PhysicalDeviceVulkan13Features, vk::PhysicalDeviceExtendedDynamicStateFeaturesEXT> featureChain{
				{ .features = { .samplerAnisotropy = true } },
				{ .shaderDrawParameters = true },
				{ .synchronization2 = true, .dynamicRendering = true },
				{ .extendedDynamicState = true }
			};


			/*Create the device-----------------------------------------------------------------------------------------*/
			float queuePrio{ 0.5f };

			uint32_t queueSize{ presentIndex == graphicsIdx ? static_cast<uint32_t>(1) : static_cast <uint32_t>(2) };

			std::vector<vk::DeviceQueueCreateInfo> deviceQueueCreateInfoArray;
			deviceQueueCreateInfoArray.reserve(queueSize);

			deviceQueueCreateInfoArray.emplace_back( vk::DeviceQueueCreateInfo {.queueFamilyIndex = graphicsIdx, .queueCount = 1, .pQueuePriorities = &queuePrio} );

			if (queueSize == 2) deviceQueueCreateInfoArray.emplace_back(vk::DeviceQueueCreateInfo{ .queueFamilyIndex = presentIndex, .queueCount = 1, .pQueuePriorities = &queuePrio });

			vk::DeviceCreateInfo deviceCreateInfo{ .pNext = &featureChain.get<vk::PhysicalDeviceFeatures2>(), .queueCreateInfoCount = queueSize, .pQueueCreateInfos = deviceQueueCreateInfoArray.data(), .enabledExtensionCount = static_cast<uint32_t>(mDeviceExtensions.size()), .ppEnabledExtensionNames = mDeviceExtensions.data()};

			mGraphicsQueueIdx = graphicsIdx;
			mPresentQueueIdx = presentIndex;

			mDevice = vk::raii::Device(mPhysicalDevice, deviceCreateInfo);
			mGraphicsQueue = vk::raii::Queue(mDevice, graphicsIdx, 0);
			mPresentQueue = vk::raii::Queue(mDevice, presentIndex, 0);

		}


		void VulkanInstance::createSurface(VkSurfaceKHR surface) {

			mSurface = vk::raii::SurfaceKHR{ mInstance, surface };

		}

		VulkanContext VulkanInstance::getVulkanContext() {
			return VulkanContext{
				.instance = &mInstance,
				.physicalDevice = &mPhysicalDevice,
				.surface = &mSurface,
				.device = &mDevice,
				.graphicsQueue = &mGraphicsQueue,
				.presentQueue = &mPresentQueue,
				.graphicsQueueIdx = mGraphicsQueueIdx,
				.presentQueueIdx = mPresentQueueIdx
			};
		}

		VulkanInstance::VulkanInstance() {
			initInstance();
			setupDebugMessenger();
			//instance.submitDebugUtilsMessageEXT(vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning,
			//	vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral, vk::DebugUtilsMessengerCallbackDataEXT{ .pMessage = physicalDevice.getProperties().deviceName });
		}

		

	}
}