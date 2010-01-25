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

    Entity(const char * buffer);
    ~Entity() {}

    bool isGood() { return m_good; }

    Direction orientation() { return m_direction; }
    void setOrientation(Direction direction) { m_direction = direction; }

    MovementMode movementMode() { return m_movementMode; }
    void setMovementMode(MovementMode movementMode) { m_movementMode = movementMode; }

    // world location of the player's contact zone
    double centerX() { return m_x + m_centerOffsetX; }
    double centerY() { return m_y + m_centerOffsetY; }
    double radius() { return m_radius; }
    void setCenter(double x, double y) { m_x = x - m_centerOffsetX; m_y = y - m_centerOffsetY; }

    double velocityX() { return m_velocityX; }
    double velocityY() { return m_velocityY; }
    void setVelocity(double x, double y) { m_velocityX = x; m_velocityY = y; }

    int layer() { return m_z; }
    void setLayer(int z) { m_z = z; }

    void draw(double screenX, double screenY);
private:
    bool m_good;

    Graphic * m_currentGraphic;

    // animations for walking, running, and standing, indexed by direction
    Graphic * m_standing[9];
    Graphic * m_walking[9];
    Graphic * m_running[9];

    Direction m_direction;
    MovementMode m_movementMode;

    // hit box (actually a circle)
    double m_centerOffsetX, m_centerOffsetY, m_radius;

    double m_x, m_y;
    double m_velocityX, m_velocityY;
    int m_z;
    double m_altitude, m_altitudeVelocity;
};

#endif

