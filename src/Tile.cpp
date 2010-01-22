#include "Tile.h"

#include "Utils.h"
#include "Gameplay.h"
#include "ResourceManager.h"

#include "SDL.h"
#include <cmath>

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

void Tile::resolveCollision(double tileX, double tileY,
                            double objectLeft, double objectTop, double objectWidth, double objectHeight,
                            double & dx, double & dy)
{
    switch (m_shape) {
    case tsSolidWall: {
            double tileCenterX = tileX + Tile::size / 2.0, tileCenterY = tileY + Tile::size / 2.0;
            double objectCenterX = objectLeft + objectWidth / 2.0, objectCenterY = objectTop + objectHeight / 2.0;
            double distanceX = objectCenterX - tileCenterX, distanceY = objectCenterY - tileCenterY;
            double absDistanceX = std::abs(distanceX), absDistanceY = std::abs(distanceY);
            if (absDistanceX < (Tile::size + objectWidth) / 2.0)
                objectLeft = tileCenterX + (distanceX < 0.0 ? -1.0 : 1.0) * objectWidth / 2.0;
            break;
        }
    case tsSolidFloor:
        // nothing
        break;
    case tsSolidHole:
        Debug::assert(false, "TODO: tile shapes");
        break;
    // floor + wall diagonals. tsDiag(pp1)(pp2)(where-pp2-is)
    case tsDiagFloorWallNW:
        Debug::assert(false, "TODO: tile shapes");
        break;
    case tsDiagFloorWallNE:
        Debug::assert(false, "TODO: tile shapes");
        break;
    case tsDiagFloorWallSE:
        Debug::assert(false, "TODO: tile shapes");
        break;
    case tsDiagFloorWallSW:
        Debug::assert(false, "TODO: tile shapes");
        break;
    // floor + rail orientations
    case tsFloorRailN:
        Debug::assert(false, "TODO: tile shapes");
        break;
    case tsFloorRailE:
        Debug::assert(false, "TODO: tile shapes");
        break;
    case tsFloorRailS:
        Debug::assert(false, "TODO: tile shapes");
        break;
    case tsFloorRailW:
        Debug::assert(false, "TODO: tile shapes");
        break;
    case tsFloorRailNE:
        Debug::assert(false, "TODO: tile shapes");
        break;
    case tsFloorRailSE:
        Debug::assert(false, "TODO: tile shapes");
        break;
    case tsFloorRailSW:
        Debug::assert(false, "TODO: tile shapes");
        break;
    case tsFloorRailNW:
        Debug::assert(false, "TODO: tile shapes");
        break;
    default:
        Debug::assert(false, "unknown tile shape");
        break;
    }
}

Tile * Tile::nullTile() {
    static Tile * s_nullTile = new Tile();
    return s_nullTile;
}
