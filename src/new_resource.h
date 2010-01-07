#ifndef NEW_RESOURCE_H
#define NEW_RESOURCE_H

enum ZoomRegionOrientaiton {
    zroSolid, zroVertical, zroHorizontal, zroForwardDiagonal, zroBackwardDiagonal
};

class ZoomRegion {
public:
    float zoomFor(int x, int y);
private:
    int x, y, w, h;
    ZoomRegionOrientaiton orientation;
    float a, b, c; // range [0.0f, 1.0f] when applicable
};

enum PhysicalPresence {
    ppHole, ppFloor, ppWall
};

enum TileShape {
    tsSolidHole, tsSolidFloor, tsSolidWall,
    // tsDiag(pp1)(pp2)(where-pp1-is)
    tsDiagWallFloorNW, tsDiagWallFloorNE, tsDiagWallFloorSE, tsDiagWallFloorSW,
};

enum SurfaceType {
    stNormal,
    stWater,
    stIce,
    stQuickSand,
};


#endif // NEW_RESOURCE_H
