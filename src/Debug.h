#ifndef _DEBUG_H_
#define _DEBUG_H_

#include <iostream>
#include <string>

#ifdef assert
// on windows, there is some macro named assert already.
// don't know what it is, but i don't want it.
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
