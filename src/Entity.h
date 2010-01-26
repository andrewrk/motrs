#ifndef _ENTITY_H_
#define _ENTITY_H_

#include "Graphic.h"

// entities are responsible for:
//  * specs (health, speed, defense, etc.)
//  * animations for moving
//  * where they are in the world
class Entity
{
public:
    // directions:
    // 0 1 2
    // 3 4 5
    // 6 7 8
    enum Direction {
        NorthWest,
        North,
        NorthEast,
        West,
        Center,
        East,
        SouthWest,
        South,
        SouthEast,
    };
    enum MovementMode {
        Stand,
        Walk,
        Run,
        Jump,
    };

    static Entity * load(const char * buffer);

    bool isGood() { return m_good; }

    Direction orientation() { return m_direction; }
    void setOrientation(Direction direction) { m_direction = direction; }

    MovementMode movementMode() { return m_movementMode; }
    void setMovementMode(MovementMode movementMode) { m_movementMode = movementMode; }

    // world location of the player's contact zone
    double centerX() { return m_centerX; }
    double centerY() { return m_centerY; }
    void setCenter(double x, double y) { m_centerX = x; m_centerY = y; }
    // radius of the hitbox (actually a circle)
    double radius() { return m_radius; }

    double velocityX() { return m_velocityX; }
    double velocityY() { return m_velocityY; }
    void setVelocity(double x, double y) { m_velocityX = x; m_velocityY = y; }

    int layer() { return m_layer; }
    void setLayer(int layer) { m_layer = layer; }

    void draw(double screenX, double screenY);

private:
    bool m_good;

    double m_centerX, m_centerY;
    const double m_radius;
    double m_velocityX, m_velocityY;
    int m_layer;
    double m_altitude, m_altitudeVelocity;

    Direction m_direction;
    MovementMode m_movementMode;

    // hit box (actually a circle)
    const double m_centerOffsetX, m_centerOffsetY;

    // animations for walking, running, and standing, indexed by direction
    Graphic * m_standing[9];
    Graphic * m_walking[9];
    Graphic * m_running[9];

    Entity(double radius, double centerOffsetX, double centerOffsetY);
    ~Entity() {}
};

#endif

