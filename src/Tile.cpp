#include "Tile.h"

#include "Utils.h"
#include "Gameplay.h"
#include "ResourceManager.h"

#include "SDL.h"

const double Tile::size = 16.0;
const int Tile::sizeInt = 16;

Tile::Tile(const char** cursor) :
    m_good(true),
    m_shape(tsSolidWall),
    m_surfaceType(stNormal),
    m_graphic(NULL)
{
    m_shape = (Shape)Utils::readInt(cursor);
    m_surfaceType = (SurfaceType)Utils::readInt(cursor);
    std::string graphicId = Utils::readString(cursor);
    m_graphic = ResourceManager::getGraphic(graphicId);
    if (m_graphic == NULL) {
        m_good = false;
        return;
    }
}

Tile::Tile() :
    m_good(true),
    m_shape(tsSolidWall),
    m_surfaceType(stNormal),
    m_graphic(NULL)
{
}

Tile::~Tile() {
}

bool Tile::isGood() {
    return m_good;
}

void Tile::draw(double screenX, double screenY) {
    m_graphic->draw(Gameplay::instance()->screen(), (int)screenX, (int)screenY);
}

Tile * Tile::nullTile() {
    static Tile * s_nullTile = new Tile();
    return s_nullTile;
}
