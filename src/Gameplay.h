#ifndef _GAMEPLAY_H_
#define _GAMEPLAY_H_

#include "SDL.h"
#include "Universe.h"
#include "Character.h"
#include "ResourceFile.h"

class Gameplay
{
public:
    Gameplay(SDL_Surface * screen, int fps);
    ~Gameplay();

    void mainLoop();
    bool isGood();
private:
    static const char * ResourceFilePath;
    static Gameplay * singleton;

    bool m_good;
    SDL_Surface * m_screen;
    int m_fps;
    int m_interval;

    Universe * m_universe;

    World * m_currentWorld;

    void processEvents();
    void updateDisplay();
    void nextFrame();

    // TODO: crappity crap below
//    typedef struct {
//        int x;
//        int y;
//    } Coord;
//    Map* curmap;
//    Character* hero;
//
//    int mapWidthPix, mapHeightPix;
//    int offsetX, offsetY;
//    Coord prevHeroLoc;
//
//    //surfaces to draw
//    SDL_Surface* dash;
//
//
//
//    int screenX(int absX);
//    int screenY(int absY);
//    int absX(int screenX);
//    int absY(int screenY);
//
//    Coord charCoord(Character* ch);
//    SDL_Rect rect(int x, int y, int w, int h);
//    int collision(SDL_Rect r1, SDL_Rect r2);
//
//
//    int isObstacle(int x, int y);
//    int ptInSquare(int x, int y, int squX, int squY);
//    int isValidMove(Character* ch, float offX, float offY);
//
//    float feetTop(Character* ch);
//    float feetLeft(Character* ch);
//    float feetRight(Character* ch);
//    float feetBottom(Character* ch);
//    float feetCenterX(Character* ch);
//    float feetCenterY(Character* ch);
//
//    void setCharFeet(Character* ch, float newX, float newY);
//    void setCharFeetX(Character* ch, float newX);
//    void setCharFeetY(Character* ch, float newY);
//
//    void initGameplay(SDL_Surface* screen);
//    void initMapTest(SDL_Surface* screen, char* mapToTest, int startX, int startY, char* charToTest);
//    int keyPressed(SDLKey key);
};



#endif

