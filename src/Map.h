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
    class TileAndLocation {
    public:
        double x, y;
        Tile * tile;
        TileAndLocation() {}
        TileAndLocation(double x, double y, Tile * tile) : x(x), y(y), tile(tile) {}
    };

    Map(const char * buffer);
    ~Map();

    bool isGood();

    void tilesAtPoint(std::vector<TileAndLocation>& tiles, double x, double y, int layer);
    void intersectingTiles(std::vector<TileAndLocation>& tiles, double left, double top, double right, double bottom, int layer);
    void draw(double screenX, double screenY, int layer);

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

    double m_x, m_y;

    void tileRange(double left, double top, double width, double height,
                   int & indexLeft, int & indexTop, int & indexRight, int & indexBottom);
};

#endif
