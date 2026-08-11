#include "vulkanInstance.hpp"
#include "extensions.hpp"

#include <map>

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


	struct DeviceScore {
		vk::raii::PhysicalDevice const* physDevice{ nullptr };
		int score;
	};


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

			/*SYNC FEATURES FOR A VALIDATION LAYER*/
			std::vector<vk::ValidationFeatureEnableEXT> validationLayerFeatureEnabled{ vk::ValidationFeatureEnableEXT::eSynchronizationValidation };

			vk::ValidationFeaturesEXT validationFeatures{ .enabledValidationFeatureCount = static_cast<uint32_t>(validationLayerFeatureEnabled.size()), .pEnabledValidationFeatures = validationLayerFeatureEnabled.data()};
			

			vk::InstanceCreateInfo createInfo{
				.pNext = mEnableValidationLayers ? &validationFeatures : nullptr,
				.pApplicationInfo = &appInfo,
				.enabledLayerCount = static_cast<uint32_t>(requiredLayers.size()),
				.ppEnabledLayerNames = requiredLayers.data(),
				.enabledExtensionCount = static_cast<uint32_t>(requiredExtensions.size()),
				.ppEnabledExtensionNames = requiredExtensions.data(),
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

		uint32_t VulkanInstance::findTransferQueue() {
			std::vector<vk::QueueFamilyProperties> queueFamilyProperties = mPhysicalDevice.getQueueFamilyProperties();
			uint32_t idx{};
			auto graphicsQueueFamilyProperty{ 
				std::ranges::find_if(queueFamilyProperties, [this, &idx](const vk::QueueFamilyProperties& props) {
					uint32_t isPresentCompatible{mPhysicalDevice.getSurfaceSupportKHR(idx, *mSurface)};
					idx++;
					return (props.queueFlags & vk::QueueFlagBits::eTransfer) != static_cast<vk::QueueFlags>(0) && (props.queueFlags & vk::QueueFlagBits::eGraphics) == static_cast<vk::QueueFlags>(0) && !isPresentCompatible;
				}) 
			};

			if (graphicsQueueFamilyProperty == queueFamilyProperties.end()) return static_cast<uint32_t>(~0);

			uint32_t transferQueue{ static_cast<uint32_t>(std::distance(queueFamilyProperties.begin(), graphicsQueueFamilyProperty) ) };

			if (graphicsQueueFamilyProperty != queueFamilyProperties.end()) {
				std::cout << "Transfer Queue found: " << transferQueue << '\n';
			}

			return transferQueue;
		}

		void VulkanInstance::pickPhysicalDevice() {

			std::vector<DeviceScore> score;

			auto devices{ mInstance.enumeratePhysicalDevices() }; /*Get physical devices*/

			for (uint32_t i{}; i < devices.size(); ++i) {
				auto props{ devices[i].getProperties()};

				DeviceScore currentScore{
					&devices[i],
					0
				};

				auto queueFamilies{ devices[i].getQueueFamilyProperties()};
				bool isSuitable{ devices[i].getProperties().apiVersion >= VK_API_VERSION_1_3};


				const auto queueFamilyIterator{
					std::ranges::find_if(queueFamilies, [](vk::QueueFamilyProperties const& queueProps) {
						return (queueProps.queueFlags & vk::QueueFlagBits::eGraphics) != static_cast<vk::QueueFlags>(0);
					})
				};

				isSuitable = isSuitable && (queueFamilies.end() != queueFamilyIterator);

				if (isSuitable) currentScore.score += 100;
				else continue;

				if (props.deviceType != vk::PhysicalDeviceType::eIntegratedGpu) currentScore.score += 50;

				auto extensions = devices[i].enumerateDeviceExtensionProperties();
				bool found{ true };

				/*Just seeing if the current physical device in the iteration can accomodate presenting on the monitor*/
				for (char const* const& extension : mDeviceExtensions) {
					auto extensionIter{ std::ranges::find_if(extensions, [extension](vk::ExtensionProperties const& props) {
						return std::strcmp(extension, props.extensionName) == 0;
					}) };

					found = found && extensionIter != extensions.end();
				}

				isSuitable = found && isSuitable;

				if (isSuitable) {
					currentScore.score += 100;
					score.emplace_back(currentScore);
				}
				else continue;

			}

			std::sort(score.begin(), score.end(), [](const DeviceScore& prev, const DeviceScore& next) {
				return prev.score > next.score;
			});

			if (score.empty()) { throw std::runtime_error{ "Mate you don't even have a proper GPU" }; }
			mPhysicalDevice = *score[0].physDevice;
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


			vk::StructureChain<vk::PhysicalDeviceFeatures2, vk::PhysicalDeviceVulkan11Features, vk::PhysicalDeviceVulkan12Features, vk::PhysicalDeviceVulkan13Features, vk::PhysicalDeviceExtendedDynamicStateFeaturesEXT> featureChain{
				{ .features = { .samplerAnisotropy = true } },
				{ .shaderDrawParameters = true },
				{ .timelineSemaphore = true },
				{ .synchronization2 = true, .dynamicRendering = true },
				{ .extendedDynamicState = true }
			};

			uint32_t transferIdx{ findTransferQueue() };

			/*Create the device-----------------------------------------------------------------------------------------*/
			float queuePrio{ 0.5f };

			uint32_t queueSize{ presentIndex == graphicsIdx ? static_cast<uint32_t>(1) : static_cast <uint32_t>(2) };
			if (transferIdx != static_cast<int>(~0)) ++queueSize;

			std::vector<uint32_t> indexes;
			if (presentIndex == graphicsIdx) indexes.push_back(graphicsIdx);
			else {
				indexes.push_back(graphicsIdx);
				indexes.push_back(presentIndex);
			}
			if (transferIdx != ~0) indexes.push_back(transferIdx);

			std::vector<vk::DeviceQueueCreateInfo> deviceQueueCreateInfoArray;
			deviceQueueCreateInfoArray.reserve(queueSize);

			std::for_each(indexes.begin(), indexes.end(), [&deviceQueueCreateInfoArray, &queuePrio](uint32_t idx) {
				deviceQueueCreateInfoArray.emplace_back(vk::DeviceQueueCreateInfo{ .queueFamilyIndex = idx, .queueCount = 1, .pQueuePriorities = &queuePrio });
			});
			
			
			vk::DeviceCreateInfo deviceCreateInfo{ .pNext = &featureChain.get<vk::PhysicalDeviceFeatures2>(), .queueCreateInfoCount = queueSize, .pQueueCreateInfos = deviceQueueCreateInfoArray.data(), .enabledExtensionCount = static_cast<uint32_t>(mDeviceExtensions.size()), .ppEnabledExtensionNames = mDeviceExtensions.data()};

			mGraphicsQueueIdx = graphicsIdx;
			mPresentQueueIdx = presentIndex;
			mTransferQueueIdx = transferIdx;

			mDevice = vk::raii::Device(mPhysicalDevice, deviceCreateInfo);
			mGraphicsQueue = vk::raii::Queue(mDevice, graphicsIdx, 0);
			mPresentQueue = vk::raii::Queue(mDevice, presentIndex, 0);
			mTransferQueue = vk::raii::Queue(mDevice, transferIdx, 0);

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
				.transferQueue = &mTransferQueue,
				.graphicsQueueIdx = mGraphicsQueueIdx,
				.presentQueueIdx = mPresentQueueIdx,
				.transferQueueIdx = mTransferQueueIdx
			};
		}

		VulkanInstance::VulkanInstance(Window &window, VulkanContext& vulkanContext) {
			initInstance();
			setupDebugMessenger();
			//instance.submitDebugUtilsMessageEXT(vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning,
			//	vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral, vk::DebugUtilsMessengerCallbackDataEXT{ .pMessage = physicalDevice.getProperties().deviceName });
			createSurface(window.createSurface(mInstance));
			pickPhysicalDevice();
			createLogicalDevice();

			vulkanContext = getVulkanContext();
		}


		

	}
}