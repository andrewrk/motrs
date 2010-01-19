#ifndef _UNIVERSE_H_
#define _UNIVERSE_H_

#include <vector>

class World;
class Map;
class Entity;

/**
 * Universe - a list of all worlds.
 */
class Universe
{
public:
    typedef struct {
        World * world;
        double x,y; // absolute coordinates in the World
        double mapX, mapY; // absolute coordinates of the Map in the World
        Map * map;
        int z; // layer
    } Location;

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
