#ifndef _WORLD_H_
#define _WORLD_H_

#include <vector>
#include "Map.h"
#include "Universe.h"

class World
{
public:
    class WorldMap {
    public:
        // where the map is in the world
        double x, y;
        // stories
        int z;
        Map * map;
        WorldMap(double x, double y, int z, Map * map)
            : x(x), y(y), z(z), map(map) {}
    };

    // create a world from data in memory
    World(const char * buffer);
    // create an empty world
    World();
    ~World();

    // did the world load ok?
    bool isGood();

    // return the Location at specified coordinates
    Universe::Location locationOf(double absoluteX, double absoluteY);

    inline std::vector<WorldMap> * maps() { return &m_maps; }

    //hax
    Map * getMap();

protected:

    bool m_good;
    std::vector<WorldMap> m_maps;
};

#endif
