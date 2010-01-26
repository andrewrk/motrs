#include "Tile.h"

#include "Utils.h"
#include "Gameplay.h"
#include "ResourceManager.h"

#include "SDL.h"
#include <cmath>

const double Tile::size = 16.0;
const int Tile::sizeInt = (int)Tile::size;

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

void Tile::draw(int screenX, int screenY) {
    if (m_graphic == NULL)
        return;
    m_graphic->draw(Gameplay::instance()->screen(), screenX, screenY);
}

bool Tile::hasMinPresence(PhysicalPresence minPresence) {
    switch (m_shape) {
    case tsSolidWall: return minPresence <= ppWall;
    case tsSolidFloor: return minPresence <= ppFloor;
    case tsSolidHole: return minPresence <= ppHole;
    case tsDiagFloorWallNW: return minPresence <= ppWall;
    case tsDiagFloorWallNE: return minPresence <= ppWall;
    case tsDiagFloorWallSE: return minPresence <= ppWall;
    case tsDiagFloorWallSW: return minPresence <= ppWall;
    case tsFloorRailN: return minPresence <= ppRail;
    case tsFloorRailE: return minPresence <= ppRail;
    case tsFloorRailS: return minPresence <= ppRail;
    case tsFloorRailW: return minPresence <= ppRail;
    case tsFloorRailNE: return minPresence <= ppRail;
    case tsFloorRailSE: return minPresence <= ppRail;
    case tsFloorRailSW: return minPresence <= ppRail;
    case tsFloorRailNW: return minPresence <= ppRail;
    default: Debug::assert(false, "Bad physical presence value"); return false;
    }
}

void Tile::resolveCircleCollision(double tileX, double tileY, double & objectCenterX, double & objectCenterY, double objectRadius) {
    switch (m_shape) {
    case tsSolidWall:
        resolveCircleOnSquare(tileX, tileY, objectCenterX, objectCenterY, objectRadius);
        break;
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

void Tile::resolveCircleOnSquare(double tileX, double tileY, double & objectCenterX, double & objectCenterY, double objectRadius) {
    // zones:
    // 0 1 2
    // 3 4 5
    // 6 7 8
    int zoneX = 1 * ((objectCenterX > tileX) + (tileX + Tile::size < objectCenterX));
    int zoneY = 3 * ((objectCenterY > tileY) + (tileY + Tile::size < objectCenterY));
    int zone = zoneX + zoneY;
    switch (zone) {
    case 0:
        resolveCircleOnPoint(tileX, tileY, objectCenterX, objectCenterY, objectRadius);
        break;
    case 1:
        objectCenterY = Utils::min(objectCenterY, tileY - objectRadius);
        break;
    case 2:
        resolveCircleOnPoint(tileX + Tile::size, tileY, objectCenterX, objectCenterY, objectRadius);
        break;
    case 3:
        objectCenterX = Utils::min(objectCenterX, tileX - objectRadius);
        break;
    case 4:
        // omg inside the wall!
        break;
    case 5:
        objectCenterX = Utils::max(objectCenterX, tileX + Tile::size + objectRadius);
        break;
    case 6:
        resolveCircleOnPoint(tileX, tileY + Tile::size, objectCenterX, objectCenterY, objectRadius);
        break;
    case 7:
        objectCenterY = Utils::max(objectCenterY, tileY + Tile::size + objectRadius);
        break;
    case 8:
        resolveCircleOnPoint(tileX + Tile::size, tileY + Tile::size, objectCenterX, objectCenterY, objectRadius);
        break;
    default:
        Debug::assert(false, "unknown zone");
    }
}

void Tile::resolveCircleOnPoint(double pointX, double pointY, double & objectCenterX, double & objectCenterY, double objectRadius) {
    double distance = Utils::distance(objectCenterX, objectCenterY, pointX, pointY);
    if (distance < objectRadius) {
        double normX = (objectCenterX - pointX) / distance;
        double normY = (objectCenterY - pointY) / distance;
        objectCenterX = pointX + normX * objectRadius;
        objectCenterY = pointY + normY * objectRadius;
    }
}

Tile * Tile::nullTile() {
    static Tile * s_nullTile = new Tile();
    return s_nullTile;
}
