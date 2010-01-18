#ifndef _MAP_H_
#define _MAP_H_

#include "Array3.h"
#include "ResourceFile.h"
#include "Tile.h"
#include "Trigger.h"
#include "Entity.h"

#include <vector>

class Map
{
public:
    Map(const char * buffer);
    ~Map();

    bool isGood();

    // draw on screen
    void draw(double screenX, double screenY, int layer);

    // width and height accessors
    inline double width(){ return m_width; }
    inline double height() { return m_height; }

private:

    enum LayerType {
        ltFull = 1,
        ltSparse = 2,
    };
    typedef struct {
        int x, y, tile;
    } SparseTile;


    bool m_good;

    std::vector<Tile*> m_palette;
    Array3<int> * m_tiles;
    std::vector<Map*> m_submaps;
    std::vector<Trigger*> m_triggers;
    std::vector<Entity*> m_entities;

    // size in absolute coordinates
    double m_x, m_y;
    double m_width, m_height; // these are computed when the map is created
};

#endif
