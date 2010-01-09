#include "Debug.h"
#include <iostream>

inline Debug::assert(bool value, std::string message)
{
    // TODO: leave this code out when compiling in release mode
    if( ! value ) {
        std::cout << message << std::endl;
        throw 0;
    }
}
