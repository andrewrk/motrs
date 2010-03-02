#ifndef _GRAPHIC_H_
#define _GRAPHIC_H_

#include <vector>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

// Graphics are things that are displayed. it can be an animation
// or a still image.
class Graphic {
public:
    // allocates and returns a new Graphic based on the buffer. will return
    // NULL if there was a problem.
    static Graphic * load(const char * buffer);

    ~Graphic();

    // draw to a surface
    void draw(sf::RenderWindow * dest, const sf::IntRect & destRect);
    void draw(sf::RenderWindow * dest, int x, int y);

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
        sf::Uint8 r;
        sf::Uint8 g;
        sf::Uint8 b;
    } RGB;

    typedef struct {
        unsigned int frameCount;
        unsigned int framesPerSecond;
        unsigned int frameWidth;
        unsigned int frameHeight;
        unsigned int imageSize; // size in bytes of the following image file
    } Header;

    sf::Image * m_image;
    sf::Sprite * m_spriteSheet;
    int m_frameCount;
    int m_fps;
    int m_offset;

    std::vector<sf::IntRect> m_spriteBounds; // boundaries of each sprite

protected: //methods
    Graphic();

    int currentFrame();
};

#endif

