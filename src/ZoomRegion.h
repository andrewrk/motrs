#ifndef _ZOOM_REGION_H_
#define _ZOOM_REGION_H_

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


#endif
