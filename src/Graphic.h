#ifndef _GRAPHIC_H_
#define _GRAPHIC_H_

#include "SDL.h"
#include "ResourceFile.h"
#include <vector>

/* Graphics are things that are displayed. it can be an animation
 * or a still image.
 */
class Graphic
{
    public:
        // create an animation or bitmap from memory
        Graphic(const char * buffer)

        // create an animation from memory and offset the animation
        Graphic(const char * buffer, int offset)
        ~Graphic();

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
        int m_offset;

        std::vector<SDL_Rect> m_spriteBounds; // boundaries of each sprite
};

#endif

