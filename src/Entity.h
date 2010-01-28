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
        JumpUp,
        JumpDown,
    };
    enum Sequence {
        Sword,
    };

    static Entity * load(const char * buffer);

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

    // altitude is for jumping and is equivalent to negative y
    double altitude() { return m_altitude; }
    void setAltitude(double value) { m_altitude = value; }
    double altitudeVelocity() { return m_altitudeVelocity; }
    void setAltitudeVelocity(double value) { m_altitudeVelocity = value; }
    void applyAltitudeVelocity() { m_altitude += m_altitudeVelocity; }

    Direction orientation() { return m_direction; }
    void setOrientation(Direction direction) { m_direction = direction; }

    MovementMode movementMode() { return m_movementMode; }
    void setMovementMode(MovementMode movementMode) { m_movementMode = movementMode; }

    // returns the next position
    void resetSequencePosition() { m_sequencePosition = 0; }
    int incrementSequencePosition() { return ++m_sequencePosition; }

    void draw(double screenX, double screenY);

private:
    double m_centerX, m_centerY;
    double m_radius;
    double m_velocityX, m_velocityY;
    int m_layer;
    double m_altitude, m_altitudeVelocity;

    Direction m_direction;
    MovementMode m_movementMode;

    // hit box (actually a circle)
    double m_centerOffsetX, m_centerOffsetY;

    // used for counting the frames in a sequence, like a sword swing
    int m_sequencePosition;

    // animations for various visible activities indexed by direction
    Graphic * m_standing[9];
    Graphic * m_walking[9];
    Graphic * m_running[9];
    Graphic * m_sword[9];

    Entity(double radius, double centerOffsetX, double centerOffsetY);
    ~Entity() {}
};

#endif

