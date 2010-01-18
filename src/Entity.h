#ifndef _ENTITY_H_
#define _ENTITY_H_

#include "Graphic.h"

#include "PropertiesResource.h"

// entities are responsible for:
//  * specs (health, speed, defense, etc.)
//  * animations for moving
//  * where they are in the world (why would an entity know what room it's in?)
class Entity
{
public:
    enum Direction {
        NorthWest,
        West,
        SouthWest,
        North,
        Center,
        South,
        NorthEast,
        East,
        SouthEast,
    };

    Entity(const char * buffer);
    ~Entity();

    // returns the position in the world
    double x() { return m_x; }
    double y() { return m_y; }

    // returns the actual position in the world of the player's feet
    double feetX() { return m_x + m_feetX; }
    double feetY() { return m_y + m_feetY; }

    // the width and height of the foot hit box area
    double feetWidth() { return m_feetW; }
    double feetHeight() { return m_feetH; }

private:
    bool m_good;

    Graphic * m_currentGraphic;

    // animations for walking, running, and standing, indexed by direction
    Graphic * m_standing[9];
    Graphic * m_walking[9];
    Graphic * m_running[9];

    int m_directionPointing;

    // hit box
    double m_feetX, m_feetY, m_feetW, m_feetH;

    double m_x;
    double m_y;

    double m_velX;
    double m_velY;
};

#endif

