#ifndef _UTILS_H_
#define _UTILS_H_

#include <iostream>
#include <sstream>
#include <map>

namespace Utils
{
    const double RadHalf = 0.70710678118654757;

    template <class T>
    T min(T x, T y) { return (x > y) ? y : x; }

    template <class T>
    T max(T x, T y) { return (x > y) ? x : y; }

    double distance(double x1, double y1, double x2, double y2);
    double distance2(double x1, double y1, double x2, double y2);

    int stringToInt(std::string value);
    std::string intToString(int value);

    std::string readString(const char ** cursor);
    int readInt(const char ** cursor);

    template <class T>
    T* readStruct(const char** cursor) {
        const char* value = *cursor;
        *cursor += sizeof(T);
        return (T*)value;
    }

    std::string trim(std::string s);
    std::string ltrim(std::string s);
    std::string rtrim(std::string s);
    bool loadProperties(std::string path, std::map<std::string, std::string> & propsMap);
}

#endif
