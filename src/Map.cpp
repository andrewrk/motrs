#include "Map.h"

#include "ResourceManager.h"

#include "Utils.h"
#include "Debug.h"

Map * Map::load(const char *buffer) {
    const char * cursor = buffer;
    int version = Utils::readInt(&cursor);
    assert(version == 4);

    int sizeX = Utils::readInt(&cursor);
    int sizeY = Utils::readInt(&cursor);

    Map * map = new Map();

    // palette
    int tileCount = Utils::readInt(&cursor);
    map->m_palette.push_back(Tile::nullTile());
    for (int i = 0; i < tileCount; i++) {
        Tile * tile = Tile::loadFromMemory(&cursor);
        map->m_palette.push_back(tile);
        if (tile == NULL) {
            delete map;
            return NULL;
        }
    }

    // layers
    int layerCount = Utils::readInt(&cursor);
    map->m_tiles = new Array3<int>(sizeX, sizeY, layerCount);
    map->m_tiles->clear(); // must be cleared for sparse layers
    for (int z = 0; z < layerCount; z++) {
        LayerType layerType = (LayerType)Utils::readInt(&cursor);
        switch (layerType) {
            case ltFull:
                for (int y = 0; y < sizeY; y++)
                    for (int x = 0; x < sizeX; x++)
                        map->m_tiles->set(x, y, z, Utils::readInt(&cursor));
                break;
            case ltSparse: {
                int tileCount = Utils::readInt(&cursor);
                for (int i = 0; i < tileCount; i++) {
                    SparseTile * sparseTile = Utils::readStruct<SparseTile>(&cursor);
                    map->m_tiles->set(sparseTile->x, sparseTile->y, z, sparseTile->tile);
                }
                break;
            }
            default: assert(false);
        }
    }

    // submaps
    int submapCount = Utils::readInt(&cursor);
    assert(submapCount == 0);

    // triggers
    int triggerCount = Utils::readInt(&cursor);
    assert(triggerCount == 0);

    // entities
    int entityCount = Utils::readInt(&cursor);
    for (int i = 0; i < entityCount; i++) {
        int x = Utils::readInt(&cursor);
        int y = Utils::readInt(&cursor);
        int layer = Utils::readInt(&cursor);
        std::string id = Utils::readString(&cursor);
        Entity * entity = ResourceManager::getEntity(id);
        if (entity == NULL) {
            delete map;
            return NULL;
        }
        entity->setCenter(x, y);
        entity->setLayer(layer);
        map->m_entities.push_back(entity);
    }

    map->calculateBoundaries();
    return map;
}

Map::Map() :
    m_palette(),
    m_tiles(NULL),
    m_submaps(),
    m_entities(),
    m_x(0.0), m_y(0.0),
    m_width(0.0), m_height(0.0),
    m_story(0)
{
}

Map::~Map() {
    for (unsigned int i = 0; i < m_palette.size(); i++)
        delete m_palette[i];
}

void Map::calculateBoundaries()
{
    // pre-calculations
    m_width = m_tiles->sizeX() * Tile::size;
    m_height = m_tiles->sizeY() * Tile::size;
}

void Map::tilesAtPoint(std::vector<TileAndLocation>& tiles, double x, double y, int layer) {
    int tileIndexX = (int)((x - m_x) / Tile::size), tileIndexY = (int)((y - m_y) / Tile::size);
    if (!(0 <= tileIndexX && tileIndexX < m_tiles->sizeX() &&
          0 <= tileIndexY && tileIndexY < m_tiles->sizeY())) {
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
            tile->draw(mapX + tileIndexX * Tile::sizeInt, mapY + tileIndexY * Tile::sizeInt);
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
    indexRight = Utils::min(tileIndexRight, m_tiles->sizeX());
    indexBottom = Utils::min(tileIndexBottom, m_tiles->sizeY());
}
