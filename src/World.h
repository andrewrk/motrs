#ifndef _WORLD_H_
#define _WORLD_H_

#include <vector>
#include "Map.h"
#include "Universe.h"

class World
{
public:
    // create a world from data in memory
    World(const char * buffer);
    // create an empty world
    World();
    ~World();

    // did the world load ok?
    bool isGood();

    // return the Location at specified coordinates
    Universe::Location locationOf(double absoluteX, double absoluteY);

    //hax
    Map * getMap();

protected:
    class WorldMap {
    public:
        double x, y;
        int z;
        Map * map;
        WorldMap(double x, double y, int z, Map * map)
            : x(x), y(y), z(z), map(map) {}
    };

    bool m_good;
    std::vector<WorldMap> m_maps;
};

#endif
