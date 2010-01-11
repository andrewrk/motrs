#include "Map.h"

#include "Utils.h"
#include "Debug.h"

#include "Gameplay.h"

Map::Map(const char * buffer) :
    m_good(true),
    m_pallet(),
    m_tiles(NULL),
    m_submaps(),
    m_triggers(),
    m_entities(),
    x(0.0), y(0.0)
{
    const char * cursor = buffer;
    Header* header = Utils::readStruct<Header>(&cursor);

    // pallet
    for (int i = 0; i < header->palletSize; i++) {
        Tile * tile = new Tile(&cursor);
        if (!tile->isGood()) {
            m_good = false;
            return;
        }
        m_pallet.push_back(tile);
    }

    // layers/tiles
    m_tiles = new Array3<int>(header->sizeX, header->sizeY, header->layerCount);
    for (int z = 0; z < header->layerCount; z++) {
        LayerType layerType = (LayerType)Utils::readInt(&cursor);
        switch (layerType) {
            case ltFull:
                for (int y = 0; y < header->sizeY; y++)
                    for (int x = 0; x < header->sizeX; x++)
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
    Debug::assert(header->submapCount == 0, "TODO support submaps");

    // triggers
    Debug::assert(header->triggerCount == 0, "TODO support triggers");

    // entities
    Debug::assert(header->entityCount == 0, "TODO support entities");
}

Map::~Map()
{

}

bool Map::isGood()
{
    return m_good;
}

void Map::draw(double screenX, double screenY, int layer) {
    double localLeft = x - screenX;
    double localTop = x - screenY;
    int tileIndexLeft = (int)(localLeft / Tile::size);
    int tileIndexTop = (int)(localTop / Tile::size);
    int tileIndexRight = tileIndexLeft + (int)(Gameplay::ScreenWidth / Tile::size);
    int tileIndexBottom = tileIndexTop + (int)(Gameplay::ScreenHeight / Tile::size);
    int tileIndexStartX = Utils::max(tileIndexLeft, 0);
    int tileIndexStartY = Utils::max(tileIndexTop, 0);
    int tileIndexEndX = Utils::min(tileIndexRight, m_tiles->sizeX);
    int tileIndexEndY = Utils::min(tileIndexBottom, m_tiles->sizeY);

    for (int tileIndexY = tileIndexStartY; tileIndexY < tileIndexEndY; tileIndexY++)
        for (int tileIndexX = tileIndexStartX; tileIndexX < tileIndexEndX; tileIndexX++)
            m_pallet[m_tiles->get(tileIndexX, tileIndexY, layer)]->draw(x + tileIndexX * Tile::size, y + tileIndexY * Tile::size);

    for (unsigned int i = 0; i < m_submaps.size(); i++)
        m_submaps[i]->draw(screenX, screenY, layer);
}
