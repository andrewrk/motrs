#include "Config.h"

#include "Utils.h"

Config * Config::s_inst = NULL;

Config::Config(int argc, char * argv[], std::string configFile) :
    m_configManager(new ConfigManager())
{
    m_configManager->addFile(configFile);
    m_configManager->addArgs(argc, argv);
}

Config * Config::instance()
{
    Debug::assert(s_inst != NULL, "Config not initialized properly");
    return s_inst;
}

void Config::initialize(int argc, char * argv[], std::string configFile)
{
    s_inst = new Config(argc, argv, configFile);
}

bool Config::fullscreen()
{
    return ! Utils::stringToBool(m_configManager->value("windowed", Utils::boolToString(false)));
}

SDLKey Config::keyNorth()
{
    return (SDLKey) Utils::stringToInt(m_configManager->value("key.north", Utils::intToString(SDLK_w)));
}

SDLKey Config::keyEast()
{
    return (SDLKey) Utils::stringToInt(m_configManager->value("key.east", Utils::intToString(SDLK_d)));
}

SDLKey Config::keySouth()
{
    return (SDLKey) Utils::stringToInt(m_configManager->value("key.south", Utils::intToString(SDLK_s)));
}

SDLKey Config::keyWest()
{
    return (SDLKey) Utils::stringToInt(m_configManager->value("key.west", Utils::intToString(SDLK_a)));
}

SDLKey Config::keyJump()
{
    return (SDLKey) Utils::stringToInt(m_configManager->value("key.jump", Utils::intToString(SDLK_SPACE)));
}

SDLKey Config::keyAttack1()
{
    return (SDLKey) Utils::stringToInt(m_configManager->value("key.attack_1", Utils::intToString(SDLK_j)));
}
