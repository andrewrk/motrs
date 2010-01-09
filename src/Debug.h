#ifndef _DEBUG_H_
#define _DEBUG_H_

#include <iostream>

namespace Debug
{
    inline void assert(bool value, std::string message)
    {
        // TODO: leave this code out when compiling in release mode
        if( ! value ) {
            std::cout << message << std::endl;
            throw 0;
        }
    }

}

#endif
