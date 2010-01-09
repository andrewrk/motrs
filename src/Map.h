#ifndef _MAP_H_
#define _MAP_H_

#include "new_resource.h"

#include "Array3.h"

#include <vector>

class Map
{
public:
    Map();
    ~Map();

private:
    Array3<int> m_tiles;
    std::vector<Tile*> m_pallet;
};

#endif // _MAP_H_
