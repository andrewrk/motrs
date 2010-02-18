#ifndef ARGUMENTPARSER_H
#define ARGUMENTPARSER_H

#include <string>
#include <map>

class ArgumentParser
{
public:
    ArgumentParser(int argc, char * argv[]);

    bool contains(const std::string & longArg, char shortArg ) const;
    bool contains(const std::string & longArg) const;
    std::string value(const std::string & longArg, const std::string & defaultValue = "");

    std::string applicationName() const { return m_applicationName; }
private:
    std::map<std::string, std::string> m_args;
    std::string m_applicationName;
};

#endif // ARGUMENTPARSER_H
