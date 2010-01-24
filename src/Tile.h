#ifndef _TILE_H_
#define _TILE_H_

#include "Graphic.h"

class Tile
{
public:
    static const double size;
    static const int sizeInt;

    /** It's not important to leave these values alone */
    enum PhysicalPresence {
        ppHole, ppFloor, ppRail, ppEmbrasure, ppWall
    };

    /** It's important to leave these values alone since they're stored in .map files */
    enum Shape {
        // solid
        tsSolidWall  = 0,
        tsSolidFloor = 1,
        tsSolidHole  = 2,
        // floor + wall diagonals. tsDiag(pp1)(pp2)(where-pp2-is)
        tsDiagFloorWallNW = 3,
        tsDiagFloorWallNE = 4,
        tsDiagFloorWallSE = 5,
        tsDiagFloorWallSW = 6,
        // floor + rail orientations
        tsFloorRailN  = 7,
        tsFloorRailE  = 8,
        tsFloorRailS  = 9,
        tsFloorRailW  = 10,
        tsFloorRailNE = 11,
        tsFloorRailSE = 12,
        tsFloorRailSW = 13,
        tsFloorRailNW = 14,
    };

    /** It's important to leave these values alones since they're stored in .map files */
    enum SurfaceType {
        stNormal    = 0,
        stWater     = 1,
        stIce       = 2,
        stQuickSand = 3,
    };

    static Tile * nullTile();


    Tile(const char** cursor);
    virtual ~Tile();

    // screenX and screenY is the absolute coordinates of 0,0 on the screen
    bool isGood();
    virtual void draw(double screenX, double screenY);
    bool hasMinPresence(PhysicalPresence minPresence);
    void resolveCircleCollision(double tileX, double tileY, double & objectCenterX, double & objectCenterY, double objectRadius);

protected:
    static void resolveCircleCollision(double tileX, double tileY, double & objectCenterX, double & objectCenterY, double objectRadius);

    bool m_good;

    Shape m_shape;
    SurfaceType m_surfaceType;

    Graphic * m_graphic;

    /** don't use this constructor. It's for making the NullTile */
    Tile();
};

#endif

