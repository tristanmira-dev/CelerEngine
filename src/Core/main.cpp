#define _CRTDBG_MAP_ALLOC


#include "celerEngine.hpp"
#include <stdlib.h>
#include <crtdbg.h>
#include <iostream>


int main() {


    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
    _CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDOUT);

    try {
        Celer::Core::CelerEngine engine;
        engine.run();
    } catch (std::exception &except) {
        std::cout << except.what();
    }
    


    return 0;
}