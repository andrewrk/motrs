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
                            double & objectCenterX, double & objectCenterY, double objectRadius,
                            int & hitDirections)
{
    switch (m_shape) {
    case tsSolidWall: {
        // 0 1 2
        // 3 4 5
        // 6 7 8
        int zoneX = 1 * ((objectCenterX > tileX) + (tileX + Tile::size < objectCenterX));
        int zoneY = 3 * ((objectCenterY > tileY) + (tileY + Tile::size < objectCenterY));
        int zone = zoneX + zoneY;
        double distance;
        int direction = zone;
        switch (zone) {
        case 0:
            distance = Utils::distance(objectCenterX, objectCenterY, tileX, tileY);
//            direction = tileX - objectCenterX < tileY - objectCenterY ? Entity::North : Entity::West;
            break;
        case 1:
            distance = tileY - objectCenterY;
            break;
        case 2:
            distance = Utils::distance(objectCenterX, objectCenterY, tileX + Tile::size, tileY);
//            direction = objectCenterX - tileX <= tileY - objectCenterY ? Entity::North : Entity::East;
            break;
        case 3:
            distance = tileX - objectCenterX;
            break;
        case 4:
            distance = 0.0;
            break;
        case 5:
            distance = objectCenterX - (tileX + Tile::size);
            break;
        case 6:
            distance = Utils::distance(objectCenterX, objectCenterY, tileX, tileY + Tile::size);
//            direction = tileX - objectCenterX <= objectCenterY - tileY ? Entity::South : Entity::West;
            break;
        case 7:
            distance = objectCenterY - (tileY + Tile::size);
            break;
        case 8:
            distance = Utils::distance(objectCenterX, objectCenterY, tileX + Tile::size, tileY + Tile::size);
//            direction = objectCenterX - tileX < objectCenterY - tileY ? Entity::South : Entity::East;
            break;
        default:
            Debug::assert(false, "unknown zone");
        }
        if (distance <= objectRadius)
            hitDirections |= 1 << direction;

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
