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

    bool isGood() { return m_good; }

    // returns the position in the world
    double x() { return m_x; }
    double y() { return m_y; }

    // returns the actual position in the world of the player's feet
    double feetX() { return m_x + m_feetOffsetX; }
    double feetY() { return m_y + m_feetOffsetY; }

    // the width and height of the foot hit box area
    double feetWidth() { return m_feetW; }
    double feetHeight() { return m_feetH; }

    void setPosition(double x, double y, int z, Direction direction);

    void draw(double screenX, double screenY);
private:
    bool m_good;

    Graphic * m_currentGraphic;

    // animations for walking, running, and standing, indexed by direction
    Graphic * m_standing[9];
    Graphic * m_walking[9];
    Graphic * m_running[9];

    Direction m_direction;

    // hit box
    double m_feetOffsetX, m_feetOffsetY, m_feetW, m_feetH;

    double m_x;
    double m_y;
    int m_z;

    double m_velX;
    double m_velY;
};

#endif

