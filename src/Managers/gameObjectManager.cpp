#include "gameObjectManager.hpp"

namespace Celer {
	namespace Managers {
		void GameObjectManager::addGameObject(Geometry::GameObjectProperties gameObject, AssetInformation asset) {

			mGameObjectProps.push_back(gameObject);

			mAssetInformation.push_back(asset);

		}
	}
}


