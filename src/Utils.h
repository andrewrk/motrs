#ifndef _UTILS_H_
#define _UTILS_H_

#include <iostream>
#include <sstream>

namespace Utils
{
    template T
    T min(T x, T y)
    {
        return (x > y) ? y : x;
    }

    template T
    T max(T x, T y)
    {
        return (x > y) ? x : y;
    }

    int stringToInt(std::string value);
    std::string intToString(int value);
};

#endif
