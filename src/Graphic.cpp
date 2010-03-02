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

    if (storageType == stBMP || storageType == stPNG) {
        // initialize
        out->m_image = new sf::Image();
        out->m_spriteSheet = new sf::Sprite();

        // load the .bmp
        bool good = out->m_image->LoadFromMemory(buffer, header->imageSize);

        if (! good) {
            std::cerr << "Error loading Graphic from memory" << std::endl;
            delete out;
            return NULL;
        }

        out->m_image->CreateMaskFromColor(sf::Color(colorKey->r, colorKey->g, colorKey->b));

        out->m_spriteSheet->SetImage(*out->m_image);
    } else {
        std::cerr << "unknown storageType: " << storageType << std::endl;
        return NULL;
    }

    // generate a rectangle for each frame
    for (int i = 0; i < out->m_frameCount; i++) {
        sf::IntRect rect;
        rect.Left = i * header->frameWidth;
        rect.Top = 0;
        rect.Right = (i+1) * header->frameWidth;
        rect.Bottom = header->frameHeight;
        out->m_spriteBounds.push_back(rect);
    }

    return out;
}

Graphic::Graphic() :
    m_image(NULL),
    m_spriteSheet(NULL),
    m_offset(0),
    m_spriteBounds()
{
}

Graphic::~Graphic()
{
    if (m_image)
        delete m_image;
    if (m_spriteSheet)
        delete m_spriteSheet;
}

// calculate which frame to draw
int Graphic::currentFrame()
{
    return (Gameplay::instance()->frameCount() * m_fps / Gameplay::instance()->fps() + m_offset) % m_frameCount;
}

void Graphic::draw(sf::RenderWindow * dest, int x, int y)
{
    int frame = currentFrame();

    sf::IntRect destRect;
    destRect.Left = x;
    destRect.Top = y;
    destRect.Right = x + m_spriteBounds[frame].GetWidth();
    destRect.Bottom = y + m_spriteBounds[frame].GetHeight();

    draw(dest, destRect);
}

void Graphic::draw(sf::RenderWindow * dest, const sf::IntRect & destRect)
{
    int frame = currentFrame();

    m_spriteSheet->SetPosition(destRect.Left, destRect.Top);
    m_spriteSheet->SetScale(destRect.GetWidth() / (float) m_spriteBounds[frame].GetWidth(),
                           destRect.GetHeight() / (float) m_spriteBounds[frame].GetHeight());
    m_spriteSheet->SetSubRect(m_spriteBounds[frame]);
    dest->Draw(*m_spriteSheet);
}

int Graphic::width()
{
    return m_spriteBounds[0].GetWidth();
}

int Graphic::height()
{
    return m_spriteBounds[0].GetHeight();
}
