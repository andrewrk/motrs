#include "Utils.h"
#include <cmath>
#include <fstream>


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

std::string Utils::trim(std::string s) {
    return ltrim(rtrim(s));
}

std::string Utils::ltrim(std::string s) {
    unsigned int goodStart = 0;
    for (unsigned int i = goodStart; i < s.size(); i++) {
        if (std::isspace(s[i]))
            continue;
        goodStart = i;
        break;
    }
    if (goodStart == 0)
        return s;
    return s.substr(goodStart);
}

std::string Utils::rtrim(std::string s) {
    unsigned int goodEnd = s.size();
    for (unsigned int i = goodEnd; i > 0; i--) {
        if (std::isspace(s[i]))
            continue;
        goodEnd = i;
        break;
    }
    if (goodEnd == s.size())
        return s;
    return s.substr(0, goodEnd);
}

bool Utils::loadProperties(std::string path, std::map<std::string, std::string> & propsMap) {
    std::ifstream in(path.c_str());
    if (!in.good()) {
        std::cerr << "file not found: \"" << path << "\"." << std::endl;
        return false;
    }
    std::string line;
    int lineNumber = 0;
    while (std::getline(in, line)) {
        lineNumber++;
        // strip comments
        unsigned int commentStart = line.find('#');
        if (commentStart != std::string::npos)
            line = line.substr(commentStart);
        // trim leading whitespace (indentation)
        line = ltrim(line);
        // skip blank lines
        if (line.size() == 0)
            continue;
        unsigned int equalsIndex = line.find('=');
        if (equalsIndex == std::string::npos) {
            std::cerr << "Parse error in file \"" << path << "\" line " << lineNumber << "." << std::endl;
            return false;
        }
        std::string key = rtrim(line.substr(0, equalsIndex));
        std::string value = trim(line.substr(equalsIndex + 1));
        propsMap[key] = value;
    }
    return true;
}
