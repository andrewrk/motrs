#ifndef _CHARACTER_H_
#define _CHARACTER_H_

class Character 
{
public:
    Character(const char * buffer);
    ~Character();

    enum Direction {
        NorthWest, West, SouthWest, North, Center, South, NorthEast, East, SouthEast
    };

    bool isGood();

    // call next frame in the current animation
    void nextFrame();
    
    // TODO: this class is just a struct it needs to actually be useful
public:

    Animation * curAnimation;
    int facing;

    Animation** walking;
    Animation** running;
    Animation** standing;

    int feetOffsetX;
    int feetOffsetY;
    int feetWidth;
    int feetHeight;

    float speed;

    float x;
    float y;
    float velX;
    float velY;
};

#endif

