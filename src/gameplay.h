#include "SDL.h"
#include "resource.h"

#ifndef GAMEPLAY_H
#define GAMEPLAY_H

typedef struct {
	int x;
	int y;
} Coord;

int screenX(int absX);
int screenY(int absY);
int absX(int screenX);
int absY(int screenY);

Coord charCoord(character* ch);
SDL_Rect rect(int x, int y, int w, int h);
int collision(SDL_Rect r1, SDL_Rect r2);


void enterMap(char* mapFile, int startX, int startY);
void enterSquare(character* ch, Coord* loc);
int isObstacle(int x, int y);
int ptInSquare(int x, int y, int squX, int squY);
int isValidMove(character* ch, float offX, float offY);

float feetTop(character* ch);
float feetLeft(character* ch);
float feetRight(character* ch);
float feetBottom(character* ch);
float feetCenterX(character* ch);
float feetCenterY(character* ch);

void setCharFeet(character* ch, float newX, float newY);
void setCharFeetX(character* ch, float newX);
void setCharFeetY(character* ch, float newY);

void updateDisplay();
void nextFrame();
void initGameplay(SDL_Surface* screen);
void initMapTest(SDL_Surface* screen, char* mapToTest, int startX, int startY, char* charToTest);
void processEvents();
int keyPressed(SDLKey key);


#endif

