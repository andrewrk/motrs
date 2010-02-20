#ifndef _TILE_H_
#define _TILE_H_

#include "Graphic.h"

class Tile
{
public: //variables
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

        // keep track of how many there are
        tsCount
    };

    /** It's important to leave these values alones since they're stored in .map files */
    enum SurfaceType {
        stNormal    = 0,
        stWater     = 1,
        stIce       = 2,
        stQuickSand = 3,

        // keep track of how many there are
        stCount
    };

public: //methods
    static Tile * loadFromMemory(const char** cursor);
    static Tile * nullTile();

    Tile(const Tile &); // copy constructor
    Tile(); // null constructor - constructs a null tile
    virtual ~Tile();

    // screenX and screenY is the absolute coordinates of 0,0 on the screen
    virtual void draw(int screenX, int screenY);
    bool hasMinPresence(PhysicalPresence minPresence);
    void resolveCircleCollision(double tileX, double tileY, double & objectCenterX, double & objectCenterY, double objectRadius);

    void setShape(Shape shape) { m_shape = shape; }
    Shape shape() { return m_shape; }
    void setSurfaceType(SurfaceType surfaceType) { m_surfaceType = surfaceType; }
    SurfaceType surfaceType() { return m_surfaceType; }

protected:
    static void resolveCircleOnSquare(double tileX, double tileY, double & objectCenterX, double & objectCenterY, double objectRadius);
    static void resolveCircleOnTriangleNW(double tileX, double tileY, double & objectCenterX, double & objectCenterY, double objectRadius);
    static void resolveCircleOnTriangleNE(double tileX, double tileY, double & objectCenterX, double & objectCenterY, double objectRadius);
    static void resolveCircleOnTriangleSE(double tileX, double tileY, double & objectCenterX, double & objectCenterY, double objectRadius);
    static void resolveCircleOnTriangleSW(double tileX, double tileY, double & objectCenterX, double & objectCenterY, double objectRadius);
    static void resolveCircleOnPoint(double pointX, double pointY, double & objectCenterX, double & objectCenterY, double objectRadius);

    Shape m_shape;
    SurfaceType m_surfaceType;

    Graphic * m_graphic;
};

#endif

