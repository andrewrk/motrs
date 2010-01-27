#include "Utils.h"
#include <cmath>

double Utils::distance(double x1, double y1, double x2, double y2) {
    return std::sqrt(Utils::distance2(x1, y1, x2, y2));
}

double Utils::distance2(double x1, double y1, double x2, double y2) {
    double dx = x2 - x1;
    double dy = y2 - y1;
    return dx * dx + dy * dy;
}

int Utils::stringToInt(std::string value) {
    std::stringstream ss;
    ss << value;
    int out;
    ss >> out;
    return out;
}

std::string Utils::intToString(int value) {
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
