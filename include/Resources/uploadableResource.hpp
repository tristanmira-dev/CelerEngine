#ifndef RESOURCES_UPLOADABLE_RESOURCE_HPP
#define RESOURCES_UPLOADABLE_RESOURCE_HPP

#include "commandBuffer.hpp"
#include "contexts.hpp"
#include "ownershipTransfer.hpp"

namespace Celer {

	namespace Resource {
		class UploadableResource {
			public:
				virtual ~UploadableResource() = default;
				UploadableResource() = default;
				UploadableResource(UploadableResource const&) = delete;
				UploadableResource& operator=(UploadableResource const&) = delete;

				virtual std::unique_ptr<OwnershipTransfer> upload(Wrapper::CommandBuffer& commandBuff, Core::VulkanContext& ctx) = 0;
		};


	}

}


#endif // RESOURCES_UPLOADABLE_RESOURCE_HPP