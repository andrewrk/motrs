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

private:
    typedef struct {
        int x;
        int y;
        int z; // story
        Map * map;
    } WorldMap;

    int m_storyCount;
    std::vector<WorldMap> m_maps;
};

#endif // _WORLD_H_
