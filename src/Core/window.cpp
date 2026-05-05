#include "window.hpp"
namespace Celer {
	namespace Core {
		void Window::initWindow(std::string windowName, int w, int h) {
			glfwInit();
			glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API); /*Dont create an opengl context, by default glfw was made for opengl*/
			glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
			window = glfwCreateWindow(mWidth, mHeight, windowName.c_str(), nullptr, nullptr);

			/*COME BACK*/
			//glfwSetWindowUserPointer(window, this);

			//glfwSetFramebufferSizeCallback(window, [](GLFWwindow* window, int width, int height) {
			//	HelloTriangleApplication* app{ reinterpret_cast<HelloTriangleApplication*>(glfwGetWindowUserPointer(window)) };

			//	app->frameBufferResized = true;

			//});
		}

		Window::Window(std::string windowName, int w, int h) : mWidth{ w }, mHeight{ h } {
			initWindow(windowName, w, h);
		}

		Window::~Window() {
			glfwDestroyWindow(window);
			glfwTerminate();
		}

		bool Window::shouldClose() {
			return glfwWindowShouldClose(window);
		}

		VkSurfaceKHR Window::createSurface(vk::raii::Instance &instance) {
			VkSurfaceKHR surface;
			glfwCreateWindowSurface(*instance, window, nullptr, &surface);
			return surface;
		}
	}
}