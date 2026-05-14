#ifndef WINDOW_HPP
#define WINDOW_HPP


namespace Celer {
	namespace Core {
		class Window {
			private:
				/*WINDOW VARS--------------------*/

				int mWidth;
				int mHeight;

				/*Window Object--------------------*/

				GLFWwindow* window{ nullptr };

				/*Private Functs--------------------*/

				void initWindow(std::string windowName, int w, int h);

			public:
				Window(std::string windowName = "Default", int w = 800, int h = 600);
				~Window();

				bool shouldClose();

				void pollEvents();

				inline GLFWwindow* getWindow() { return window; }

				VkSurfaceKHR createSurface(vk::raii::Instance& instance);
		};
	}
}


#endif // WINDOW_HPP