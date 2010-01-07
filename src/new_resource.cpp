
#include "new_resource.h"
#include <cstdlib>

float ZoomRegion::zoomFor(int x, int y) {
    if (orientation == zroSolid)
        return a;
    float u = (x - this->x) / (float)w;
    float v = (y - this->y) / (float)h;
    float s;
    switch (orientation) {
    case zroVertical:         s = v;               break;
    case zroHorizontal:       s = u;               break;
    case zroForwardDiagonal:  s = (u + v) / 2;     break;
    case zroBackwardDiagonal: s = (u + 1 - v) / 2; break;
    default:
        exit(1);
    }
    switch (orientation) {
    case zroVertical:
    case zroHorizontal:
        return (1.0f - s) * a + s * b;
    case zroForwardDiagonal:
    case zroBackwardDiagonal:
        return (s < 0.5f) ?
                ((1.0f - 2.0f * s) * a + 2.0f * s * b) :
                ((2.0f - 2.0f * s) * b + (2.0f * s - 1.0f) * c);
    default:
        exit(1);
        return 0.0f;
    }
}
