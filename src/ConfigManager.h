#ifndef CONFIGMANAGER_H
#define CONFIGMANAGER_H

#include <map>
#include <string>

// ConfigManager brings together arguments and config files
class ConfigManager
{
public:
    ConfigManager();

    // adding configuration - the most recent thing you add will overwrite if
    // there is a name conflict.

    // reads C-style command line arguments and adds them to the configuration
    bool addArgs(int argc, char * argv[]);

    // reads an INI file format and fills out a propsMap hash
    // use "section.property" as a key in the map to get the value
    // returns true if there were no errors adding the file
    bool addFile(std::string filename);

    // get a value from config
    std::string value(std::string key, std::string defaultValue = "");

    // the command used to start the program (only applies if you addArgs)
    std::string command();
private:
    std::map<std::string, std::string> m_map;

    std::string m_command;
};

#endif // CONFIGMANAGER_H
