#ifndef _CHARACTER_H_
#define _CHARACTER_H_

#include "Graphic.h"

#include "PropertiesResource.h"

// characters are responsible for:
//  * health
//  * animations for moving
//  * where they are in the world (why would character know what room it's in?)
class Character : public PropertiesResource
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

    Character(const char * buffer);
    ~Character();

    // returns the position in the world
    double x() { return m_x; }
    double y() { return m_y; }

    // returns the actual position in the world of the player's feet
    double feetX() { return m_x + m_feetOffsetX; }
    double feetY() { return m_y + m_feetOffsetY; }

    // the width and height of the foot hit box area
    double feetWidth() { return m_feetWidth; }
    double feetHeight() { return m_feetHeight; }

private:
    Graphic * m_currentGraphic;

    // animations for walking, running, and standing, indexed by direction
    Graphic * m_walking[9];
    Graphic * m_running[9];
    Graphic * m_standing[9];

    int m_directionPointing;

    // hit box offset
    double m_feetX, m_feetY, m_feetW, m_feetH;

    double m_speed;

    double m_x;
    double m_y;

    double m_velX;
    double m_velY;

    // health
    double m_hp;
    double m_maxHp;
};

#endif

