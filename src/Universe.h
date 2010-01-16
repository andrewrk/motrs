#ifndef _UNIVERSE_H_
#define _UNIVERSE_H_

#include "PropertiesResource.h"

#include "World.h"
#include <vector>

class World;

/**
 * Universe - a list of all worlds.
 */
class Universe
{
public:
    Universe(const char * buffer);
    ~Universe();

    bool isGood();
    int worldCount();
    World * firstWorld();

private:
    std::vector<World *> m_worlds;
    bool m_good;
    int m_startWorld;
    int m_startX;
    int m_startY;
    int m_startZ;
};

#endif
