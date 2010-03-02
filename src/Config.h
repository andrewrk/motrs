#ifndef CONFIG_H
#define CONFIG_H

#include "ConfigManager.h"
#include "Debug.h"
#include "Input.h"

#include <string>

// Config is a single instance class that contains typed methods for each
// configuration variable.
class Config
{
public:
    static Config * instance();

    // initialize with command line arguments and a configuration file
    static void initialize(int argc, char * argv[], std::string configFile);

    // settings. priority: 1. command line argument 2. config file 3. default
    bool fullscreen();

    // keys
    Input::KeyCode keyNorth();
    Input::KeyCode keyEast();
    Input::KeyCode keySouth();
    Input::KeyCode keyWest();
    Input::KeyCode keyJump();
    Input::KeyCode keyAttack1();

private: //variables
    static Config * s_inst;

    ConfigManager * m_configManager;
private: //methods
    Config(int argc, char * argv[], std::string configFile);
};

#endif // CONFIG_H
