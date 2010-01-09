#ifndef _WORLD_H_
#define _WORLD_H_

#include <vector>
#include "ResourceFile.h"
#include "Map.h"

class World
{
public:
    // create a world from data in memory
    World(ResourceFile* resourceFile, std::string resourceName);
    ~World();

    // did the world load ok?
    bool isGood();

private:
    class WorldMap {
    public:
        int x, y, z;
        Map * map;
        WorldMap(int x, int y, int z, Map * map)
            : x(x), y(y), z(z), map(map) {}
    };

    bool m_good;
    int m_storyCount;
    std::vector<WorldMap> m_maps;
};

#endif
