#ifndef _TILE_H_
#define _TILE_H_

#include "PropertiesResource.h"
#include "Graphic.h"

class Tile
{
public:
    static const double size;

    /** It's not important to leaves these values alone */
    enum PhysicalPresence {
        ppHole, ppFloor, ppRail, ppEmbrasure, ppWall
    };

    /** It's important to leaves these values alone since they're stored in .map files */
    enum Shape {
        // solid
        tsSolidWall  = 0x00,
        tsSolidFloor = 0x01,
        tsSolidHole  = 0x02,
        // floor + wall diagonals. tsDiag(pp1)(pp2)(where-pp2-is)
        tsDiagFloorWallNW = 0x10,
        tsDiagFloorWallNE = 0x11,
        tsDiagFloorWallSE = 0x12,
        tsDiagFloorWallSW = 0x13,
        // floor + rail orientations
        tsFloorRailN  = 0x20,
        tsFloorRailE  = 0x21,
        tsFloorRailS  = 0x22,
        tsFloorRailW  = 0x23,
        tsFloorRailNE = 0x24,
        tsFloorRailSE = 0x25,
        tsFloorRailSW = 0x26,
        tsFloorRailNW = 0x27,
    };

    /** It's important to leaves these values alone since they're stored in .map files */
    enum SurfaceType {
        stNormal    = 0,
        stWater     = 1,
        stIce       = 2,
        stQuickSand = 3,
    };



    Tile(const char** cursor);
    ~Tile();

    bool isGood();
    void draw(double screenX, double screenY);

private:
    enum PropertyNames {
        PropertyGraphic,
    };

    static std::map<std::string, int> s_propertyNames;

    bool m_good;

    Shape m_shape;
    SurfaceType m_surfaceType;

    Graphic * m_graphic;
    // TODO int offset
    //
};

#endif

