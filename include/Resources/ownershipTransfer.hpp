#ifndef RESOURCE_ACQUIREABLE_RESOURCE_HPP
#define RESOURCE_ACQUIREABLE_RESOURCE_HPP

namespace Celer {

	namespace Resource {

		class OwnershipTransfer {
			public:
				OwnershipTransfer() = default;
				virtual ~OwnershipTransfer() = default;
				virtual void acquire(Wrapper::CommandBuffer& commandBuff, Core::VulkanContext& ctx) = 0;
		};

	}


}




#endif // RESOURCE_ACQUIREABLE_RESOURCE_HPP