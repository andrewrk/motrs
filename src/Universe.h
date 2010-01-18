#ifndef _UNIVERSE_H_
#define _UNIVERSE_H_

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
    World * startWorld();

    Entity * player() { return m_player; }

private:
    std::vector<World *> m_worlds;
    bool m_good;
    int m_startWorld;

    Entity * m_player;
};

#endif
