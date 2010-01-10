#include "Tile.h"

#include "Utils.h"

const int Tile::size = 16;

Tile::Tile(const char** cursor) :
    m_good(true),
    m_shape(tsSolidWall),
    m_surfaceType(stNormal),
    m_image(NULL)
{
    m_shape = (Shape)Utils::readInt(cursor);
    m_surfaceType = (SurfaceType)Utils::readInt(cursor);
    int propCount = Utils::readInt(cursor);
    for (int i = 0; i < propCount; i++) {
        std::string key = Utils::readString(cursor);
        std::string value = Utils::readString(cursor);
    }
}

bool Tile::isGood() {
    return m_good;
}
