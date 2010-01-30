#ifndef _WORLD_H_
#define _WORLD_H_

#include <vector>
#include <set>
#include "Map.h"
#include "Universe.h"

class World
{
public:
    static World * load(const char * buffer);
    World(const char * buffer);
    // create an empty world
    World();
    ~World();

    // did the world load ok?
    bool isGood();

    // return the Location at specified coordinates
    Universe::Location locationOf(double absoluteX, double absoluteY);

    std::vector<Map*> * maps() { return &m_maps; }

    // Position of the world
    double left() { return m_left;}
    double top() {return m_top;}
    double width() {return m_width;}
    double height() {return m_height;}
protected:

    bool m_good;
    std::vector<Map*> m_maps;

    double m_width;
    double m_height;
    double m_top;
    double m_left;
    void calculateBoundaries();
};

#endif
