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

std::string Utils::readString(char ** cursor) {
    int length = readInt(cursor);
    char* buffer = new char[length];
    for (int i = 0; i < length; i++)
        buffer[i] = *(*cursor)++;
    std::string returnValue = buffer;
    delete buffer;
    return returnValue;
}

int Utils::readInt(char ** cursor) {
    int value = *(int*)*cursor;
    *cursor += sizeof(int);
    return value;
}
