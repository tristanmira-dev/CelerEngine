#ifndef RENDER_TEXTUREMANAGER_HPP
#define RENDER_TEXTUREMANAGER_HPP

#include "image.hpp"
#include "deviceMemManager.hpp"

namespace Celer {
	namespace Render {
		class TextureManager {
			private:
				Core::DeviceMemoryManager& mDeviceManager;
				//Wrapper::OwnedImage mImage;

				Wrapper::OwnedImageCollection mImageCollection;
				Core::Memory mMemory;

			public:
				TextureManager(Core::DeviceMemoryManager& deviceManager);
		};
	}
}



#endif // RENDER_TEXTUREMANAGER_HPP