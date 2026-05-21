#ifndef WINDOW_HPP
#define WINDOW_HPP


namespace Celer {
	namespace Core {
		class Window {
			private:
				/*WINDOW VARS--------------------*/

				int mWidth;
				int mHeight;

				bool isResized{ false };

				/*Window Object--------------------*/

				GLFWwindow* window{ nullptr };


				/*Private Functs--------------------*/

				void initWindow(std::string windowName, int w, int h);
				inline void setDimen(int width, int height) {
					mWidth = width;
					mHeight = height;
				}
				

			public:
				Window(std::string windowName = "Default", int w = 800, int h = 600);
				~Window();

				bool shouldClose();

				void pollEvents();

				void setResizeEvent();

				void setWindowUserPointer();

				inline void getDimen(int &width, int &height) {
					width = mWidth;
					height = mHeight;
				}

				inline bool getResized() {
					return isResized;
				}

				inline void resized(bool condition) {
					isResized = condition;
				}

				inline GLFWwindow* getWindow() { return window; }

				VkSurfaceKHR createSurface(vk::raii::Instance& instance);

				void getFrameBufferSize(int &width, int &height);
		};
	}
}


#endif // WINDOW_HPP