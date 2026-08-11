#ifndef RENDER_TEXTUREMANAGER_HPP
#define RENDER_TEXTUREMANAGER_HPP

#include "image.hpp"
#include "deviceMemManager.hpp"
#include "uploadManager.hpp"

namespace Celer {
	namespace Render {
		class TextureManager {
			private:
				Core::DeviceMemoryManager& mDeviceMemoryManager;
				//Wrapper::OwnedImage mImage;
				Wrapper::CommandBuffer mCommandBuff;

				Wrapper::OwnedImageCollection mImageCollection;
				std::vector<Core::Memory> mMemory;

			public:
				TextureManager(Core::DeviceMemoryManager& deviceManager, Core::VulkanContext& ctx);

				void addTexture(char const* file, Core::VulkanContext& ctx, Core::UploadManager& uploadManager);
		};
	}
}



#endif // RENDER_TEXTUREMANAGER_HPP