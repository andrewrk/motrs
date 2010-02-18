#include "ArgumentParser.h"

#include <iostream>

ArgumentParser::ArgumentParser(int argc, char * argv[]) :
    m_args(),
    m_applicationName(argv[0])
{
    for(int i=1; i<argc; ++i) {
        // gimme C++ string
        std::string arg(argv[i]);
        // determine if this is a long or short parameter
        if( arg.size() == 0 || arg.at(0) != '-' || arg.size() == 1 ) {
            // skip this invalid parameter
            std::cerr << "ArgumentParser::ArgumentParser: invalid parameter: "
                << arg << std::endl;
            continue;
        } else if( arg.at(1) == '-' && arg.size() > 2 ) {
            // long parameter
            int pos = arg.find('=');
            if( pos == -1 ) {
                std::string param = arg.substr(2, arg.size() - 2);
                m_args.insert(std::pair<std::string, std::string>(param, ""));
            } else {
                std::string param = arg.substr(2, pos - 2);
                std::string value = arg.substr(pos + 1, arg.size() - (pos+1));
                m_args.insert(std::pair<std::string, std::string>(param, value));
            }
        } else if( arg.size() == 2) {
            // short parameter
            m_args.insert(std::pair<std::string, std::string>(arg.substr(1,1), ""));
        } else {
            std::cerr << "ArgumentParser::ArgumentParser: invalid parameter: "
                << arg << std::endl;
        }
    }
}

bool ArgumentParser::contains(const std::string & longArg, char shortArg ) const
{
    return m_args.count(longArg) > 0 || m_args.count(std::string(1, shortArg)) > 0;
}

bool ArgumentParser::contains(const std::string & longArg) const
{
    return m_args.count(longArg) > 0;
}

std::string ArgumentParser::value(const std::string & longArg, const std::string & defaultValue)
{
    return (m_args.count(longArg) > 0) ? m_args.at(longArg) : defaultValue;
}
