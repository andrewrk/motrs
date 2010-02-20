#include "ConfigManager.h"

#include "Utils.h"

#include <fstream>

ConfigManager::ConfigManager()
{
}

bool ConfigManager::addFile(std::string filename)
{
    std::ifstream in(filename.c_str());
    if (!in.good()) {
        std::cerr << "file not found: \"" << filename << "\"." << std::endl;
        return false;
    }
    std::string line;
    std::string header = "";
    int lineNumber = 0;
    while (std::getline(in, line)) {
        lineNumber++;
        // strip comments
        size_t commentStart = line.find(';');
        if (commentStart != std::string::npos)
            line = line.substr(0, commentStart);
        // trim leading whitespace (indentation)
        line = Utils::ltrim(line);
        // skip blank lines
        if (line.size() == 0)
            continue;
        // look for section header
        size_t leftBracket = line.find('[');
        size_t rightBracket = line.find(']');
        if( leftBracket == std::string::npos ||
            rightBracket == std::string::npos )
        {
            // look for equals sign
            unsigned int equalsIndex = line.find('=');
            if (equalsIndex == std::string::npos) {
                std::cerr << "Parse error in file \"" << filename << "\" line " << lineNumber << "." << std::endl;
                return false;
            }
            std::string key = Utils::rtrim(line.substr(0, equalsIndex));
            std::string value = Utils::trim(line.substr(equalsIndex + 1));

            if( header.size() > 0 )
                key = header + "." + key;

            m_map[key] = value;
        } else {
            // section header
            header = line.substr(leftBracket + 1, rightBracket - leftBracket - 1);
        }
    }
    return true;
}

bool ConfigManager::addArgs(int argc, char * argv[])
{
    bool success = true;
    m_command = std::string(argv[0]);
    for(int i=1; i<argc; ++i) {
        // gimme C++ string
        std::string arg(argv[i]);
        // determine if this is a long or short parameter
        if( arg.size() == 0 || arg.at(0) != '-' || arg.size() == 1 ) {
            // skip this invalid parameter
            std::cerr << "ConfigManager::addArgs: invalid parameter: "
                << arg << std::endl;
            success = false;
            continue;
        } else if( arg.at(1) == '-' && arg.size() > 2 ) {
            // long parameter
            int pos = arg.find('=');
            if( pos == -1 ) {
                std::string param = arg.substr(2, arg.size() - 2);
                m_map[param] = "true";
            } else {
                std::string param = arg.substr(2, pos - 2);
                std::string value = arg.substr(pos + 1, arg.size() - (pos+1));
                m_map[param] = "true";
            }
        } else if( arg.size() == 2) {
            // short parameter
            //m_map.insert(std::pair<std::string, std::string>(arg.substr(1,1), "true"));
            // short parameters not supported
            std::cerr << "ConfigManager::addArgs: short parameters not supported" << std::endl;
            success = false;
            continue;
        } else {
            std::cerr << "ConfigManager::addArgs: invalid parameter: "
                << arg << std::endl;
            success = false;
        }
    }
    return success;
}

std::string ConfigManager::command()
{
    return m_command;
}

std::string ConfigManager::value(std::string key, std::string defaultValue)
{
    return (m_map.count(key) > 0) ? m_map[key] : defaultValue;
}
