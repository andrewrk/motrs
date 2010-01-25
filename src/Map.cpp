#include "Map.h"

#include "Utils.h"
#include "Debug.h"

Map::Map(const char * buffer) :
    m_good(true),
    m_palette(),
    m_tiles(NULL),
    m_submaps(),
    m_triggers(),
    m_entities(),
    m_x(0.0), m_y(0.0)
{
    const char * cursor = buffer;
    int version = Utils::readInt(&cursor);
    if (version != 1) {
        std::cerr << "Unsupported Map version: " << version << std::endl;
        m_good = false;
        return;
    }

    int sizeX = Utils::readInt(&cursor);
    int sizeY = Utils::readInt(&cursor);

    // pallet
    int tileCount = Utils::readInt(&cursor);
    m_palette.push_back(Tile::nullTile());
    for (int i = 0; i < tileCount; i++) {
        Tile * tile = new Tile(&cursor);
        if (!tile->isGood()) {
            m_good = false;
            return;
        }
        m_palette.push_back(tile);
    }

    // layers
    int layerCount = Utils::readInt(&cursor);
    m_tiles = new Array3<int>(sizeX, sizeY, layerCount);
    m_tiles->clear(); // must be cleared for sparse layers
    for (int z = 0; z < layerCount; z++) {
        LayerType layerType = (LayerType)Utils::readInt(&cursor);
        switch (layerType) {
            case ltFull:
                for (int y = 0; y < sizeY; y++)
                    for (int x = 0; x < sizeX; x++)
                        m_tiles->set(x, y, z, Utils::readInt(&cursor));
                break;
            case ltSparse: {
                int tileCount = Utils::readInt(&cursor);
                for (int i = 0; i < tileCount; i++) {
                    SparseTile * sparseTile = Utils::readStruct<SparseTile>(&cursor);
                    m_tiles->set(sparseTile->x, sparseTile->y, z, sparseTile->tile);
                }
                break;
            }
            default: Debug::assert(false, "unkown layer type");
        }
    }

    // submaps
    int submapCount = Utils::readInt(&cursor);
    Debug::assert(submapCount == 0, "TODO support submaps");

    // triggers
    int triggerCount = Utils::readInt(&cursor);
    Debug::assert(triggerCount == 0, "TODO support triggers");

    // entities
    int entityCount = Utils::readInt(&cursor);
    Debug::assert(entityCount == 0, "TODO support entities");


    // pre-calculations
    m_width = sizeX * Tile::size;
    m_height = sizeY * Tile::size;
}

Map::Map() :
    m_good(true),
    m_palette(),
    m_tiles(NULL),
    m_submaps(),
    m_triggers(),
    m_entities(),
    m_x(0.0), m_y(0.0)
{
}

Map::~Map()
{
    for (unsigned int i = 0; i < m_palette.size(); i++)
        delete m_palette[i];
}

bool Map::isGood()
{
    return m_good;
}

void Map::tilesAtPoint(std::vector<TileAndLocation>& tiles, double x, double y, int layer) {
    int tileIndexX = (int)((x - m_x) / Tile::size), tileIndexY = (int)((y - m_y) / Tile::size);
    if (!(0 <= tileIndexX && tileIndexX < m_tiles->sizeX &&
          0 <= tileIndexY && tileIndexY < m_tiles->sizeY)) {
        double tileX = m_x + tileIndexX * Tile::size, tileY = m_y + tileIndexY * Tile::size;
        Tile * tile = m_palette[m_tiles->get(tileIndexX, tileIndexY, layer)];
        tiles.push_back(TileAndLocation(tileX, tileY, tile));
    }

    for (unsigned int i = 0; i < m_submaps.size(); i++)
        m_submaps[i]->tilesAtPoint(tiles, x, y, layer);
}

void Map::intersectingTiles(std::vector<TileAndLocation>& tiles, double centerX, double centerY, double apothem,
                            int layer, Tile::PhysicalPresence minPresence)
{
    int tileIndexStartX, tileIndexStartY, tileIndexEndX, tileIndexEndY;
    tileRange(centerX - apothem, centerY - apothem, apothem * 2.0, apothem * 2.0,
              tileIndexStartX, tileIndexStartY, tileIndexEndX, tileIndexEndY);

    for (int tileIndexY = tileIndexStartY; tileIndexY < tileIndexEndY; tileIndexY++) {
        for (int tileIndexX = tileIndexStartX; tileIndexX < tileIndexEndX; tileIndexX++) {
            Tile * tile = m_palette[m_tiles->get(tileIndexX, tileIndexY, layer)];
            if (tile->hasMinPresence(minPresence))
                tiles.push_back(TileAndLocation(tileIndexX * Tile::size + m_x, tileIndexY * Tile::size + m_y, tile));
        }
    }
}

void Map::draw(double screenX, double screenY, double screenWidth, double screenHeight, int layer) {
    int tileIndexStartX, tileIndexStartY, tileIndexEndX, tileIndexEndY;
    tileRange(screenX, screenY, screenWidth, screenHeight, tileIndexStartX, tileIndexStartY, tileIndexEndX, tileIndexEndY);

    int mapX = (int)(m_x - screenX), mapY = (int)(m_y - screenY);
    for (int tileIndexY = tileIndexStartY; tileIndexY < tileIndexEndY; tileIndexY++) {
        for (int tileIndexX = tileIndexStartX; tileIndexX < tileIndexEndX; tileIndexX++) {
            int tileIndex = m_tiles->get(tileIndexX, tileIndexY, layer);
            Tile * tile = m_palette[tileIndex];
            tile->draw(mapX + tileIndexX * (int)Tile::size, mapY + tileIndexY * (int)Tile::size);
        }
    }

    for (unsigned int i = 0; i < m_submaps.size(); i++)
        m_submaps[i]->draw(screenX, screenY, screenWidth, screenHeight, layer);
}

void Map::tileRange(double left, double top, double width, double height,
                    int & indexLeft, int & indexTop, int & indexRight, int & indexBottom)
{
    double localLeft = left - m_x;
    double localTop = top - m_y;
    double localRight = localLeft + width;
    double localBottom = localTop + height;
    int tileIndexLeft = (int)(localLeft / Tile::size);
    int tileIndexTop = (int)(localTop / Tile::size);
    int tileIndexRight = (int)(localRight / Tile::size) + 1;
    int tileIndexBottom = (int)(localBottom / Tile::size) + 1;
    indexLeft = Utils::max(tileIndexLeft, 0);
    indexTop = Utils::max(tileIndexTop, 0);
    indexRight = Utils::min(tileIndexRight, m_tiles->sizeX);
    indexBottom = Utils::min(tileIndexBottom, m_tiles->sizeY);
}
