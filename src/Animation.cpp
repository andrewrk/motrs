#include "Animation.h"

#include <cmath>

Animation::Animation(const char * buffer, int hostFps) :
    m_spriteSheet(NULL),
    m_currentFrame(0),
    m_hostFps(hostFps),
    m_framesPassed(0),
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
    
Animation::~Animation()
{
    if( m_spriteSheet )
        SDL_FreeSurface(m_spriteSheet);
}

void Animation::nextFrame(int frameSkip)
{
    m_framesPassed += (m_hostFps / m_fps) * frameSkip;
    
    float framesToAdvance = std::floor(m_framesPassed);
    m_framesPassed -= framesToAdvance;
    advanceFrame((int)framesToAdvance);
}

void Animation::advanceFrame(int frameSkip)
{
    m_currentFrame = (m_currentFrame + frameSkip) % m_frameCount;
}

void Animation::draw(SDL_Surface * dest, SDL_Rect * destRect)
{
    SDL_BlitSurface(m_spriteSheet, &m_spriteBounds[m_currentFrame], 
        dest, destRect);
}

bool Animation::isGood()
{
    return m_spriteSheet != NULL;
}

