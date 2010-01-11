#include "Graphic.h"

#include "Gameplay.h"

#include <cmath>

Graphic::Graphic(const char * buffer) :
    m_spriteSheet(NULL),
    m_spriteBounds()
{
    // TODO: make graphic able to take a bitmap here as well as animations
    
    // load the animation from buffer
    Header * header = (Header*)buffer;

    // properties
    m_frameCount = header->frameCount;
    m_fps = header->framesPerSecond;
    
    // sprite sheet
    SDL_RWops* rw = SDL_RWFromConstMem(buffer+sizeof(Header),
        header->bitmapSize);
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

// calculate which frame to draw
int Graphic::currentFrame()
{
    return (Gameplay::instance()->frameCount() * m_fps /
        Gameplay::instance()->fps() + m_offset) % m_frameCount;
}

void Graphic::draw(SDL_Surface * dest, int x, int y)
{
    int frame = currentFrame();

    SDL_Rect destRect;
    destRect.x = x;
    destRect.y = y;
    destRect.w = m_spriteBounds[frame].w;
    destRect.h = m_spriteBounds[frame].h;

    SDL_BlitSurface( m_spriteSheet, &m_spriteBounds[currentFrame()], 
        dest, &destRect);
}

void Graphic::draw(SDL_Surface * dest, SDL_Rect * destRect)
{
    SDL_BlitSurface( m_spriteSheet, &m_spriteBounds[currentFrame()], 
        dest, destRect);
}

bool Graphic::isGood()
{
    return m_spriteSheet != NULL;
}

