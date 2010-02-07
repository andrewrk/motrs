#include "Physics.h"

#include "Utils.h"
#include "Debug.h"

void Physics::circleAndCircle(double cx1, double cy1, double r1, double cx2, double cy2, double r2, double & out_dx, double & out_dy) {
    double distance = Utils::distance(cx1, cy1, cx2, cy2);
    double minDistance = r1 + r2;
    double overlap = minDistance - distance;
    if (overlap > 0.0) {
        double dx = cx2 - cx1, dy = cy2 - cy1;
        double normalX = dx / distance, normalY = dy / distance;
        out_dx = normalX * overlap;
        out_dy = normalY * overlap;
    } else {
        out_dx = 0;
        out_dy = 0;
    }
}

void Physics::squareAndCircle(double cx1, double cy1, double a1, double cx2, double cy2, double r2, double & out_dx, double & out_dy) {
    // zones:
    // 0 1 2
    // 3 4 5
    // 6 7 8
    int zoneX = 1 * ((cx1 - a1 < cx2) + (cx1 + a1 < cx2));
    int zoneY = 3 * ((cy1 - a1 < cy2) + (cy1 + a1 < cy2));
    int zone = zoneX + zoneY;
    switch (zone) {
    case 0:
        pointAndCircle(cx1 - a1 , cy1 - a1 , cx2, cy2, r2, out_dx, out_dy);
        break;
    case 1:
        out_dx = 0.0;
        out_dy = Utils::min(0.0, (cy1 - a1) - (cy2 + r2));
        break;
    case 2:
        pointAndCircle(cx1 + a1, cy1 - a1 , cx2, cy2, r2, out_dx, out_dy);
        break;
    case 3:
        out_dx = Utils::min(0.0, (cx1 - a1) - (cx2 + r2));
        out_dy = 0.0;
        break;
    case 4:
        // omg inside the square! gtfo!
        pointAndCircle(cx1, cy1, cx2, cy2, r2 + a1, out_dx, out_dy);
        break;
    case 5:
        out_dx = Utils::max(0.0, (cx1 + a1) - (cx2 - r2));
        out_dy = 0.0;
        break;
    case 6:
        pointAndCircle(cx1 - a1 , cy1 + a1, cx2, cy2, r2, out_dx, out_dy);
        break;
    case 7:
        out_dx = 0.0;
        out_dy = Utils::max(0.0, (cy1 + a1) - (cy2 - r2));
        break;
    case 8:
        pointAndCircle(cx1 + a1, cy1 + a1, cx2, cy2, r2, out_dx, out_dy);
        break;
    default:
        Debug::assert(false, "unknown zone");
    }
}

void Physics::squareAndSquare(double cx1, double cy1, double a1, double cx2, double cy2, double a2, double &out_dx, double &out_dy) {

}

void Physics::pointAndCircle(double px1, double py1, double cx2, double cy2, double r2, double &out_dx, double &out_dy) {
    double distance = Utils::distance(px1, py1, cx2, cy2);
    double overlap = r2 - distance;
    if (overlap > 0.0) {
        double normX = (cx2 - px1) / distance;
        double normY = (cy2 - py1) / distance;
        out_dx = normX * overlap;
        out_dy = normY * overlap;
    } else {
        out_dx = 0.0;
        out_dy = 0.0;
    }
}
