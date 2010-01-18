#include "Map.h"

#include "Utils.h"
#include "Debug.h"

#include "Gameplay.h"

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
    for (int z = 0; z < layerCount; z++) {
        LayerType layerType = (LayerType)Utils::readInt(&cursor);
        switch (layerType) {
            case ltFull:
                for (int y = 0; y < sizeY; y++)
                    for (int x = 0; x < sizeX; x++)
                        m_tiles->set(x, y, z, Utils::readInt(&cursor));
                break;
            case ltSparse: {
                m_tiles->clear();
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

Map::~Map()
{
    for (unsigned int i = 0; i < m_palette.size(); i++)
        delete m_palette[i];
}

bool Map::isGood()
{
    return m_good;
}

void Map::draw(double screenX, double screenY, int layer) {
    double localLeft =  screenX - m_x;
    double localTop =  screenY - m_x;
    int tileIndexLeft = (int)(localLeft / Tile::size);
    int tileIndexTop = (int)(localTop / Tile::size);
    int tileIndexRight = tileIndexLeft + (int)(Gameplay::instance()->screenWidth() / Tile::size) + 1;
    int tileIndexBottom = tileIndexTop + (int)(Gameplay::instance()->screenHeight() / Tile::size) + 1;
    int tileIndexStartX = Utils::max(tileIndexLeft, 0);
    int tileIndexStartY = Utils::max(tileIndexTop, 0);
    int tileIndexEndX = Utils::min(tileIndexRight, m_tiles->sizeX);
    int tileIndexEndY = Utils::min(tileIndexBottom, m_tiles->sizeY);

    for (int tileIndexY = tileIndexStartY; tileIndexY < tileIndexEndY; tileIndexY++) {
        for (int tileIndexX = tileIndexStartX; tileIndexX < tileIndexEndX; tileIndexX++) {
            int tileIndex = m_tiles->get(tileIndexX, tileIndexY, layer);
            Tile * tile = m_palette[tileIndex];
            tile->draw(m_x + tileIndexX * Tile::size - screenX, m_y + tileIndexY * Tile::size - screenY);
        }
    }

    for (unsigned int i = 0; i < m_submaps.size(); i++)
        m_submaps[i]->draw(screenX, screenY, layer);
}
