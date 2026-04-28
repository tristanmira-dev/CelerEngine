#include "extensions.hpp"

namespace Celer {
	std::vector<const char*> getRequiredInstanceExtensions(bool enableValidationLayers) {
		uint32_t glfwExtensionCount{};
		auto glfwExtensions{ glfwGetRequiredInstanceExtensions(&glfwExtensionCount) };

		auto extensions{ std::vector<const char*>(glfwExtensions, glfwExtensions + glfwExtensionCount) };

		if (enableValidationLayers) extensions.push_back(vk::EXTDebugUtilsExtensionName);

		return  extensions;
	}
}

