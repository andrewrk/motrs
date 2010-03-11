#ifndef _WORLD_H_
#define _WORLD_H_

#include <vector>
#include <set>
#include "Map.h"
#include "Universe.h"

class World
{
public:
    // load a world from memory. returns NULL on error
    static World * load(const char * buffer);
    // create an empty world
    World();
    ~World();

    // return the Location at specified coordinates
    Universe::Location locationOf(double absoluteX, double absoluteY);

    std::vector<Map*> * maps() { return &m_maps; }
private:
    std::vector<Map*> m_maps;
};

#endif
