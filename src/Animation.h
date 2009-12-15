#ifndef _ANIMATION_H_
#define _ANIMATION_H_

#include "SDL.h"

#include <vector>

class Animation
{
    public:
        // create an animation from resource data. fps is how many times
        // nextFrame() will be called per second
        Animation(const char * buffer, int hostFps);
        ~Animation();

        // call this whenever you advance a frame (you are the host)
        // it will compensate for fps differences between you and the animation.
        void nextFrame(int frameSkip = 1);

        // override fps compensation and go to the next frame in the
        // animation.
        void advanceFrame(int frameSkip = 1);

        // draw to an SDL surface
        void draw(SDL_Surface * dest, SDL_Rect * destRect);

        // check if the animation loaded ok
        bool isGood();

    private:
        typedef struct {
            Uint8 r;
            Uint8 g;
            Uint8 b;
        } RGB;

        typedef struct {
            RGB colorKey;
            Uint32 frameCount;
            Uint32 framesPerSecond;
            Uint32 frameWidth;
            Uint32 frameHeight;
            Uint32 bitmapSize; // size in bytes of the following .bmp file
        } Header;

        SDL_Surface * m_spriteSheet;
        int m_frameCount;
        int m_currentFrame;
        int m_fps;
        int m_hostFps;

        float m_framesPassed; // how many frames have passed by
        std::vector<SDL_Rect> m_spriteBounds; // boundaries of each sprite
};

#endif

