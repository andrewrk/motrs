#include "Utils.h"

int Utils::stringToInt(std::string value)
{
    std::stringstream ss;
    ss << value;
    int out;
    ss >> out;
    return out;
}

std::string Utils::intToString(int value)
{
    std::stringstream ss;
    ss << value;
    return ss.str();
}

std::string Utils::readString(const char ** cursor) {
    int size = readInt(cursor);
    std::string value(*cursor, size);
    *cursor += size;
    return value;
}

int Utils::readInt(const char ** cursor) {
    int value = *(int*)*cursor;
    *cursor += sizeof(int);
    return value;
}
