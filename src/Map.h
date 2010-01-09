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
    Map(ResourceFile * resourceFile, std::string resourceName);
    ~Map();

    bool isGood();

    inline int width() { return m_tiles->sizeX; }
    inline int height() { return m_tiles->sizeY; }
    inline int layerCount() { return m_tiles->sizeZ; }

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
    std::vector<Tile*> m_pallet;
    Array3<int> * m_tiles;
    std::vector<Map*> m_submaps;
    std::vector<Trigger*> m_triggers;
    std::vector<Entity*> m_entities;
};

#endif // _MAP_H_
