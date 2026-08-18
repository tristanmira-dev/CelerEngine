
#include "vulkanBuffer.hpp"
#include "texturemanager.hpp"

namespace {
	
}

namespace Celer {
	namespace Render {
		TextureManager::TextureManager(Core::DeviceMemoryManager& deviceManager, Core::VulkanContext& ctx) : mDeviceMemoryManager{ deviceManager }, mCommandBuff(*ctx.device, 1, ctx.transferQueueIdx) {

			


		}

		


		void TextureManager::addTexture(char const* file, Core::VulkanContext& ctx, Core::UploadManager& uploadManager) {
			int texWidth, texHeight, texChannels;
			stbi_uc* pixels = stbi_load(file, &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);

			if (!pixels)
			{
				throw std::runtime_error("failed to load texture image!");
			}	

			vk::Format format{ vk::Format::eR8G8B8A8Srgb };

			mImageCollection.addOwnedImage(format, *ctx.device, vk::Extent3D{ static_cast<uint32_t>(texWidth), static_cast<uint32_t>(texHeight), 1 }, vk::ImageTiling::eOptimal, vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eSampled);

			vk::DeviceSize imageSize = texWidth * texHeight * 4;

			vk::MemoryRequirements imageMemReq{ mImageCollection.back().getImageMemoryReq() };

			


			mMemory.push_back(mDeviceMemoryManager.allocateImageMemory(imageMemReq.size, imageMemReq.alignment));

			assert(mMemory.back().getSize() == imageMemReq.size && "WARNING, DIFFERENT MEMORY FROM THE IMAGE MEM REQUIREMENT");

			//ADD ACTUAL UPLOAD REQUEST TO QUEUE (UPLOAD MANAGER)
			uploadManager.addImageResource(ctx, Core::ResourceType::IMAGE, mMemory.back(), static_cast<void*>(pixels), &mImageCollection.back().imageRef(), texWidth, texHeight);

			/*align*/

			std::cout << imageMemReq.size << " " << imageMemReq.alignment;

			/*bind image*/
			
			mDeviceMemoryManager.bindImage(mImageCollection.back().getImage(), mMemory.back());


			/*set image view, make sure the copy happens to the local buffer beforehand*/
			mImageCollection.back().setImageView(*ctx.device, format);


			stbi_image_free(pixels);


		}
	}
}
