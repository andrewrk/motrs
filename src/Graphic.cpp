#include "Graphic.h"

#include <cmath>

Graphic::Graphic(const char * buffer, int hostFps) :
    m_spriteSheet(NULL),
    m_spriteBounds()
{
    // load the animation from buffer
    Header * header = (Header*)buffer;

    // properties
    m_frameCount = header->frameCount;
    m_fps = header->framesPerSecond;
    
    // sprite sheet
    SDL_RWops* rw = SDL_RWFromConstMem(buffer+sizeof(Header), header->bitmapSize);
    SDL_Surface * temp = SDL_LoadBMP_RW(rw, 1);

    if( ! temp ) 
        return; // error. isGood() will return false.


    m_spriteSheet = SDL_DisplayFormat(m_spriteSheet);
    SDL_FreeSurface(temp);
    SDL_SetColorKey(m_spriteSheet, SDL_SRCCOLORKEY|SDL_RLEACCEL, 
        SDL_MapRGB(m_spriteSheet->format, header->colorKey.r,
            header->colorKey.g, header->colorKey.b));

    // generate SDL_Rects for each frame
    for(int i=0; i<m_frameCount; ++i) {
        SDL_Rect rect;
        rect.x = i * header->frameWidth;
        rect.y = 0;
        rect.w = header->frameWidth;
        rect.h = header->frameHeight;
        m_spriteBounds.push_back(rect);
    }
}
    
Graphic::~Graphic()
{
    if( m_spriteSheet )
        SDL_FreeSurface(m_spriteSheet);
}

void Graphic::nextFrame(int frameSkip)
{
    m_framesPassed += (m_hostFps / m_fps) * frameSkip;
    
    float framesToAdvance = std::floor(m_framesPassed);
    m_framesPassed -= framesToAdvance;
    advanceFrame((int)framesToAdvance);
}

void Graphic::advanceFrame(int frameSkip)
{
    m_currentFrame = (m_currentFrame + frameSkip) % m_frameCount;
}

void Graphic::draw(SDL_Surface * dest, SDL_Rect * destRect)
{
    // calculate which frame to draw
    SDL_BlitSurface(m_spriteSheet, &m_spriteBounds[m_currentFrame], 
        dest, destRect);
}

bool Graphic::isGood()
{
    return m_spriteSheet != NULL;
}

