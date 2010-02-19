#ifndef CONFIG_H
#define CONFIG_H

#include "ConfigManager.h"
#include "Debug.h"

#include "SDL.h"

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
    SDLKey keyNorth();
    SDLKey keyEast();
    SDLKey keySouth();
    SDLKey keyWest();
    SDLKey keyJump();
    SDLKey keyAttack1();

private: //variables
    static Config * s_inst;

    ConfigManager * m_configManager;
private: //methods
    Config(int argc, char * argv[], std::string configFile);
};

#endif // CONFIG_H
