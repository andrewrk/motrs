#include "Utils.h"

int Utils::stringToInt(std::string value)
{
    std::sstream ss;
    ss << value;
    int out;
    ss >> out;
    return out;
}

std::string Utils::intToString(int value)
{
    std::sstream ss;
    ss << value;
    return ss.str();
}
