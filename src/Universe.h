#ifndef _UNIVERSE_H_
#define _UNIVERSE_H_

#include "PropertiesResource.h"

#include "World.h"
#include <vector>

/**
 * Universe - a list of all worlds.
 */
class Universe : public PropertiesResource
{
public:
    Universe(ResourceFile * resourceFile, std::string resourceName);
    ~Universe();

    World* firstWorld();
private:
    std::vector<World *> m_worlds;
};

#endif
