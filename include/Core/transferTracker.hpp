#ifndef CORE_TRANSFER_TRACKER_HPP
#define CORE_TRANSFER_TRACKER_HPP

#include "texturemanager.hpp"

#include "renderer.hpp"

#include <variant>

namespace Celer {

	namespace Core {

		struct ImageResource {

		};


		struct BufferResource {

		};

		class ResourceSync {
			private:
				
			public:
				ResourceSync();


		};

		class TransferManager {
			private:
				std::list<std::variant<ImageResource, BufferResource>> mPendingAcquires;

			public:
				TransferManager() = default;
		};




	}

}

#endif // CORE_TRANSFER_TRACKER_HPP