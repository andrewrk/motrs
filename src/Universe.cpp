#include "Universe.h"

Universe::Universe(ResourceFile * resourceFile, std::string resourceName)
    : PropertiesResource(resourceFile, resourceName)
{
    // TODO populate m_worlds with property("worlds")
    Variant firstWorld = property("tmp.world");
    m_worlds.push_back(new World(resourceFile, firstWorld.stringValue));
}

Universe::~Universe()
{

}

World * Universe::firstWorld() {
    return m_worlds[0];
}
