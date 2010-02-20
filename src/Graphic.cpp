#include "Graphic.h"

#include "Gameplay.h"
#include "Utils.h"
#include "Debug.h"

#include <cmath>

Graphic * Graphic::load(const char * buffer)
{

    int versionNumber = Utils::readInt(&buffer);

    if (versionNumber != 1) {
        std::cerr << "Wrong Graphic version number: " << versionNumber << std::endl;
        return NULL;
    }

    int graphicType = Utils::readInt(&buffer);
    int storageType = Utils::readInt(&buffer);

    RGB * colorKey = Utils::readStruct<RGB>(&buffer);
    Header * header = Utils::readStruct<Header>(&buffer);

    Graphic * out = new Graphic();

    out->m_frameCount = header->frameCount;
    out->m_fps = header->framesPerSecond;

    if (graphicType == gtImage && out->m_frameCount != 1) {
        std::cerr << "still images can't have more than 1 frame." << std::endl;
        delete out;
        return NULL;
    }

    if (storageType == stBMP) {
        // load the .bmp
        SDL_RWops * rw = SDL_RWFromConstMem(buffer, header->imageSize);
        SDL_Surface * temp = SDL_LoadBMP_RW(rw, 1);
        Debug::assert(temp != NULL, "lalala");
        if (temp == NULL) {
            std::cerr << "Error turning Graphic buffer into SDL_Surface" << std::endl;
            delete out;
            return NULL;
        }

        out->m_spriteSheet = SDL_DisplayFormat(temp);
        SDL_FreeSurface(temp);
        SDL_SetColorKey(out->m_spriteSheet, SDL_SRCCOLORKEY|SDL_RLEACCEL,
            SDL_MapRGB(out->m_spriteSheet->format, colorKey->r,
                colorKey->g, colorKey->b));
    } else if (storageType == stPNG) {
        // TODO: implement PNG support
        Debug::assert(false, "TODO: add PNG support in Graphic");
    } else {
        std::cerr << "unknown storageType: " << storageType << std::endl;
        return NULL;
    }

    // generate SDL_Rects for each frame
    for (int i = 0; i < out->m_frameCount; i++) {
        SDL_Rect rect;
        rect.x = i * header->frameWidth;
        rect.y = 0;
        rect.w = header->frameWidth;
        rect.h = header->frameHeight;
        out->m_spriteBounds.push_back(rect);
    }
    return out;
}

Graphic::Graphic() :
    m_spriteSheet(NULL),
    m_offset(0),
    m_spriteBounds()
{

}

Graphic::~Graphic() {
    if (m_spriteSheet)
        SDL_FreeSurface(m_spriteSheet);
}

// calculate which frame to draw
int Graphic::currentFrame() {
    return (Gameplay::instance()->frameCount() * m_fps / Gameplay::instance()->fps() + m_offset) % m_frameCount;
}

void Graphic::draw(SDL_Surface * dest, int x, int y) {
    int frame = currentFrame();

    SDL_Rect destRect;
    destRect.x = x;
    destRect.y = y;
    destRect.w = m_spriteBounds[frame].w / 4;
    destRect.h = m_spriteBounds[frame].h / 4;

    SDL_BlitSurface(m_spriteSheet, &m_spriteBounds[frame], dest, &destRect);
}

void Graphic::draw(SDL_Surface * dest, SDL_Rect * destRect) {
    SDL_BlitSurface(m_spriteSheet, &m_spriteBounds[currentFrame()], dest, destRect);
}

int Graphic::width()
{
    return m_spriteBounds[0].w;
}

int Graphic::height()
{
    return m_spriteBounds[0].h;
}
