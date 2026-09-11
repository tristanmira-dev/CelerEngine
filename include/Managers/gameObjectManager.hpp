#ifndef MANAGERS_GAME_OBJECT_MANAGER_HPP
#define MANAGERS_GAME_OBJECT_MANAGER_HPP

#include "descriptors.hpp"

namespace Celer {

	namespace Managers {

		struct AssetInformation {
			uint32_t textureIndex;
			uint32_t meshIndex;
		};
		
		class GameObjectManager {

			

			public:
				std::vector<Geometry::GameObjectProperties> mGameObjectProps;
				std::vector<AssetInformation> mAssetInformation;
				void addGameObject(Geometry::GameObjectProperties gameObject, AssetInformation asset);
				



		};

	}


}





#endif