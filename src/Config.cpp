#include "Config.h"

#include "Utils.h"

Config * Config::s_inst = NULL;

Config::Config(int argc, char * argv[], std::string configFile) :
    m_configManager(new ConfigManager())
{
    m_configManager->addFile(configFile);
    m_configManager->addArgs(argc, argv);
}

Config::Config(std::string configFile) :
    m_configManager(new ConfigManager())
{
    m_configManager->addFile(configFile);
}

Config::~Config()
{
    delete m_configManager;
    s_inst = NULL;
}

Config * Config::instance()
{
    assert(s_inst != NULL);
    return s_inst;
}

void Config::initialize(int argc, char * argv[], std::string configFile)
{
    delete s_inst;
    s_inst = new Config(argc, argv, configFile);
}

void Config::initialize(std::string configFile)
{
    delete s_inst;
    s_inst = new Config(configFile);
}

bool Config::fullscreen()
{
    return ! Utils::stringToBool(m_configManager->value("windowed", Utils::boolToString(false)));
}

Input::KeyCode Config::keyNorth()
{
    return (Input::KeyCode) Utils::stringToInt(
        m_configManager->value("key.north", Utils::intToString(Input::kcW)));
}

Input::KeyCode Config::keyEast()
{
    return (Input::KeyCode) Utils::stringToInt(
        m_configManager->value("key.east", Utils::intToString(Input::kcD)));
}

Input::KeyCode Config::keySouth()
{
    return (Input::KeyCode) Utils::stringToInt(
        m_configManager->value("key.south", Utils::intToString(Input::kcS)));
}

Input::KeyCode Config::keyWest()
{
    return (Input::KeyCode) Utils::stringToInt(
        m_configManager->value("key.west", Utils::intToString(Input::kcA)));
}

Input::KeyCode Config::keyJump()
{
    return (Input::KeyCode) Utils::stringToInt(
        m_configManager->value("key.jump", Utils::intToString(Input::kcSpace)));
}

Input::KeyCode Config::keyAttack1()
{
    return (Input::KeyCode) Utils::stringToInt(
        m_configManager->value("key.attack_1", Utils::intToString(Input::kcJ)));
}
