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
    ppHole, ppFloor, ppGuardRail, ppEmbrasure, ppWall
};

enum TileShape {
    tsSolidHole, tsSolidFloor, tsSolidWall,
    // floor + wall diagonals. tsDiag(pp1)(pp2)(where-pp2-is)
    tsDiagFloorWallNW, tsDiagFloorWallNE, tsDiagFloorWallSE, tsDiagFloorWallSW,
    // floor + rail orientations
    tsFloorRailN, tsFloorRailE, tsFloorRailS, tsFloorRailW,
    tsFloorRailNE, tsFloorRailSE, tsFloorRailSW, tsFloorRailNW,
};

enum SurfaceType {
    stNormal,
    stWater,
    stIce,
    2tQuickSand,
};


#endif // NEW_RESOURCE_H
