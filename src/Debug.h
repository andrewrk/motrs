#ifndef _DEBUG_H_
#define _DEBUG_H_

#include <iostream>
#include <string>

#ifdef assert
#undef assert
#endif

namespace Debug
{
    inline void assert(bool value, std::string message)
    {
#ifndef RELEASE
        if( ! value ) {
            std::cerr << message << std::endl;
            throw 0;
        }
#endif
    }

}

#endif
