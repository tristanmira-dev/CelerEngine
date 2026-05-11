#include "image.hpp"

namespace Celer {

	namespace Wrapper {

		
		/*Image DEFINITIONS!*/




		/*ImageCollection DEFINITIONS!*/



		void ImageCollection::clear() {
			mImageView.clear();
			mImage.clear();
			mOwnedImage.clear();
		}

		ImageCollection::~ImageCollection() {
			clear();
		}

	}

}

