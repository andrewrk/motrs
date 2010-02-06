#ifndef _PHYSICS_H_
#define _PHYSICS_H_

class Physics {
public:
    // out_dx and out_dy get written to with the vector object 2 would have to move to resolve the collision
    static void circleAndCircle(double cx1, double cy1, double r1, double cx2, double cy2, double r2, double & out_dx, double & out_dy);
    static void squareAndCircle(double cx1, double cy1, double a1, double cx2, double cy2, double r2, double & out_dx, double & out_dy);
    static void squareAndSquare(double cx1, double cy1, double a1, double cx2, double cy2, double a2, double & out_dx, double & out_dy);
    static void pointAndCircle(double px1, double py1, double cx2, double cy2, double r2, double & out_dx, double & out_dy);

};

#endif
