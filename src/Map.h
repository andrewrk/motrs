#ifndef _MAP_H_
#define _MAP_H_

#include "Array3.h"
#include "ResourceFile.h"
#include "Tile.h"

#include <vector>

class Map
{
public:
    Map(ResourceFile * resourceFile, std::string resourceName);
    ~Map();

    bool isGood();

    inline int width() { return m_tiles.sizeX; }
    inline int height() { return m_tiles.sizeY; }
    inline int layerCount() { return m_tiles.sizeZ; }

private:
    Array3<int> m_tiles;
    std::vector<Tile*> m_pallet;
};

#endif // _MAP_H_
