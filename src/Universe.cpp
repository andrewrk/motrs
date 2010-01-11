#include "Universe.h"

Universe::Universe(const char * buffer)
    : PropertiesResource(buffer)
{
    if (!isGood())
        return;
    // TODO populate m_worlds with property("worlds")
    Variant firstWorld = property("tmp.world");
    World * world = ResourceManager::getWorld(firstWorld.stringValue);
    m_worlds.push_back(world);
}

Universe::~Universe()
{

}

World * Universe::firstWorld() {
    return m_worlds[0];
}
