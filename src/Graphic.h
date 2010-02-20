#ifndef _GRAPHIC_H_
#define _GRAPHIC_H_

#include "SDL.h"
#include <vector>

// Graphics are things that are displayed. it can be an animation
// or a still image.
class Graphic {
public:
    // allocates and returns a new Graphic based on the buffer. will return
    // NULL if there was a problem.
    static Graphic * load(const char * buffer);

    ~Graphic();

    // draw to an SDL surface
    void draw(SDL_Surface * dest, SDL_Rect * destRect);
    void draw(SDL_Surface * dest, int x, int y);

    // get the size
    int width();
    int height();

protected: //variables
    /*  storage format:
        Uint32 GraphicType
        Uint32 StorageType
        RGB colorKey
        Header
        Image data
    */

    enum GraphicType {
        gtAnimation = 0, // spritesheet
        gtImage = 1, // still frame
    };

    enum StorageType {
        stBMP = 0,
        stPNG = 1,
    };

    typedef struct {
        Uint8 r;
        Uint8 g;
        Uint8 b;
    } RGB;

    typedef struct {
        Uint32 frameCount;
        Uint32 framesPerSecond;
        Uint32 frameWidth;
        Uint32 frameHeight;
        Uint32 imageSize; // size in bytes of the following image file
    } Header;

    SDL_Surface * m_spriteSheet;
    int m_frameCount;
    int m_fps;
    int m_offset;

    std::vector<SDL_Rect> m_spriteBounds; // boundaries of each sprite

protected: //methods
    Graphic();

    int currentFrame();
};

#endif

