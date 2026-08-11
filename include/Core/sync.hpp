#ifndef CORE_SYNC_HPP
#define CORE_SYNC_HPP
#include "contexts.hpp"
#include "image.hpp"
#include "commandBuffer.hpp"

namespace Celer {

	namespace Core {

		void transitionLayout(Celer::Wrapper::CommandBuffer& commandBuff, vk::raii::Image& img, vk::ImageLayout oldLayout, vk::ImageLayout newLayout);

		class FrameContext {
			
			public:

				FrameContext(Core::VulkanContext& ctx);

				enum class SemaphoreType {
					BINARY,
					TIMELINE
				};

				struct SemaphoreObject {
					SemaphoreType mType;
					vk::raii::Semaphore mSemaphore = nullptr;
				};

				SemaphoreObject mFrameSyncObject;
				uint64_t mTimelineCount{};


				std::vector<SemaphoreObject> mSignalSemaphores;
				std::vector<uint64_t> mTimelineCounts;


				vk::raii::Semaphore mResourcePending = nullptr;

				void addSemaphore(SemaphoreType semaphoreType, Core::VulkanContext& ctx);


		};


	}

}


#endif // CORE_SYNC_HPP