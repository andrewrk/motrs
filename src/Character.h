#ifndef _CHARACTER_H_
#define _CHARACTER_H_

#include "Graphic.h"

// characters are responsible for:
// * health
// * animations for moving
// * where they are in the world
class Character 
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

    Character(ResourceFile * resourceFile, std::string resourceName);
    ~Character();

    // if the character loaded from the resource file ok
    bool isGood();

    // returns the position in the world
    inline double x() { return m_x; }
    inline double y() { return m_y; }

    // returns the actual position in the world of the player's feet
    inline double feetX() { return m_x + m_feetOffsetX; }
    inline double feetY() { return m_y + m_feetOffsetY; }

    // the width and height of the foot hit box area
    inline double feetWidth() { return m_feetWidth; }
    inline double feetHeight() { return m_feetHeight; }

private:
    Graphic * m_currentGraphic;

    // animations for walking, running, and standing, indexed by direction
    Graphic ** m_walking;
    Graphic ** m_running;
    Graphic ** m_standing;

    int m_directionPointing;

    // hit box offset
    double m_feetOffsetX, m_feetOffsetY;
    double m_feetWidth, m_feetHeight;
    
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

