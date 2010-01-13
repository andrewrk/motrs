#include "Universe.h"

Universe::Universe(const char * buffer)
    : PropertiesResource(buffer)
{
    if (!isGood())
        return;
    // TODO populate m_worlds with property("worlds")
    std::string firstWorldId = property("tmp.world").stringValue;
    World * world = ResourceManager::getWorld(firstWorldId);
    m_worlds.push_back(world);
}

Universe::~Universe()
{

}

World * Universe::firstWorld() {
    return m_worlds[0];
}
