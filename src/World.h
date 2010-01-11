#ifndef _WORLD_H_
#define _WORLD_H_

#include <vector>
#include "Map.h"

class World
{
public:
    // create a world from data in memory
    World(const char * buffer);
    ~World();

    // did the world load ok?
    bool isGood();
    Map * getMap();

private:
    class WorldMap {
    public:
        int x, y, z;
        Map * map;
        WorldMap(int x, int y, int z, Map * map)
            : x(x), y(y), z(z), map(map) {}
    };

    bool m_good;
    std::vector<WorldMap> m_maps;
};

#endif
