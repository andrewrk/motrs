#ifndef _UNIVERSE_H_
#define _UNIVERSE_H_

#include "PropertiesResource.h"

#include "World.h"
#include <vector>

class World;

/**
 * Universe - a list of all worlds.
 */
class Universe : public PropertiesResource
{
public:
    Universe(const char * buffer);
    ~Universe();

    World * firstWorld();
private:
    std::vector<World *> m_worlds;
};

#endif
