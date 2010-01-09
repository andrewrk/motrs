#ifndef _NEW_RESOURCE_H_
#define _NEW_RESOURCE_H_

enum PhysicalPresence {
    ppHole, ppFloor, ppRail, ppEmbrasure, ppWall
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
    stQuickSand,
};

typedef struct {
    TileShape shape;
    SurfaceType surfaceType;
} Tile;


#endif
