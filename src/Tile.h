#ifndef _TILE_H_
#define _TILE_H_

#include "PropertiesResource.h"
#include "Animation.h"

class Tile : public PropertiesResource
{
public:
    static const int size;

    enum PhysicalPresence {
        ppHole, ppFloor, ppRail, ppEmbrasure, ppWall
    };

    enum Shape {
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

    Tile(ResourceFile * resourceFile, std::string resourceName);
    ~Tile();

    
private:
    Shape m_shape;
    SurfaceType m_surfaceType;
    
    // display
    Animation * m_animation;

    SDL_Surface * m_bitmap; 
    Uint32 m_colorKey;
    bool m_useColorKey;
};

#endif

