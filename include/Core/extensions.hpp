#ifndef EXTENSIONS_HPP
#define EXTENSIONS_HPP

#include <GLFW/glfw3.h>

namespace Celer {

	std::vector<const char*> getRequiredInstanceExtensions(bool enableValidationLayers);


}

#endif // EXTENSIONS_HPP