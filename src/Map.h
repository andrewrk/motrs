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

    void draw(double screenX, double screenY, int layer);

private:
    typedef struct {
        int palletSize;
        int sizeX, sizeY, layerCount;
        int submapCount;
        int triggerCount;
        int entityCount;
    } Header;

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

    double m_x, m_y;
};

#endif
