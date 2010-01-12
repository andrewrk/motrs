#include "Gameplay.h"

#include "ResourceManager.h"
#include "Input.h"
#include "Tile.h"
#include "Debug.h"
#include "os_config.h"

#include <cmath>

#include <iostream>

#ifndef RELEASE
const char * Gameplay::ResourceFilePath = "resources.dat";
#else
const char * Gameplay::ResourceFilePath = RESOURCE_DIR "/resources.dat";
#endif

Gameplay * Gameplay::s_inst= NULL;

double const Gameplay::ScreenWidth = 800.0;
double const Gameplay::ScreenHeight = 600.0;

Gameplay::Gameplay(SDL_Surface * screen, int fps) :
    m_good(true),
    m_screen(screen),
    m_fps(fps),
    m_interval(1000/fps), //frames per second -> miliseconds
    m_universe(NULL),
    m_currentWorld(NULL)
{
    Debug::assert(s_inst == NULL, "only one Gameplay allowed");
    s_inst = this;
    m_universe = ResourceManager::loadUniverse(ResourceFilePath, "main.universe");
    m_good = m_universe->isGood();
    if (!m_good) {
        std::cerr << "failed to load universe" << std::endl;
        return;
    }
    m_currentWorld = m_universe->firstWorld();
    // TODO more here
}

Gameplay::~Gameplay() {
    delete m_universe;
    s_inst = NULL;
}

void Gameplay::mainLoop() {
    Uint32 next_time = 0;
    while (true) {
        processEvents(); //input

        //set up an interval
        Uint32 now = SDL_GetTicks();
        if (now >= next_time) {
            SDL_Flip(m_screen); //show the frame that is already drawn
            nextFrame(); //main gameplay loop
            updateDisplay(); //begin drawing next frame

            next_time = now + m_interval;
        }

        SDL_Delay(1); //give up the cpu
    }
}

bool Gameplay::isGood() {
    return m_good;
}

void Gameplay::processEvents() {
    Input::refresh();
    //handle all events
    SDL_Event event;
    while(SDL_PollEvent(&event)){
        switch(event.type){
            case SDL_QUIT:
                exit(0);
        }
    }
}


void Gameplay::nextFrame () {
//	//which keys are down
//	Uint8* keystate;
//	int dirX, dirY;
//	float angle;
//	Coord heroLoc;
//	int i;
//	int slow;
//
//	keystate = SDL_GetKeyState(NULL);
//
//	//physics
//
//	//which direction to go
//	dirX = 0;
//	dirY = 0;
//	if( keystate[SDLK_LEFT]  || keystate[SDLK_a] ) dirX--;
//	if( keystate[SDLK_RIGHT] || keystate[SDLK_d] ) dirX++;
//	if( keystate[SDLK_UP]    || keystate[SDLK_w] ) dirY--;
//	if( keystate[SDLK_DOWN]  || keystate[SDLK_s] ) dirY++;
//
//	if( dirX == 0 && dirY == 0){
//		//stand still
//		hero->velX = 0;
//		hero->velY = 0;
//
//		hero->curAnimation = hero->standing[hero->facing];
//	} else {
//		//run in a direction
//		angle = std::atan2(dirY,dirX);
//		hero->velX = hero->speed * cos(angle);
//		hero->velY = hero->speed * sin(angle);
//
//		hero->facing = (dirX+1)*3 + (dirY+1);
//		hero->curAnimation = hero->running[hero->facing];
//	}
//
//	//step to next frame in animation
//    hero->nextFrame();
//
//	//momentum
//	if( isValidMove( hero, hero->velX, 0 )  )
//		hero->x += hero->velX;
//	else {
//		slow = hero->velX / abs(hero->velX);
//		for(i=1;i<=abs(hero->velX) && isValidMove( hero, slow, 0 );i++)
//			hero->x += slow;
//	}
//
//	if( isValidMove( hero, 0, hero->velY )  )
//		hero->y += hero->velY;
//	else {
//		slow = hero->velY / abs(hero->velY);
//		for(i=1;i<=abs(hero->velY) && isValidMove( hero, 0, slow );i++)
//			hero->y += slow;
//	}
//
//	//special tiles
//	heroLoc = charCoord(hero);
//
////	if( heroLoc.x != prevHeroLoc.x || heroLoc.y != prevHeroLoc.y  )
////		enterSquare(hero, &heroLoc);
//
//
//	//calculate screen offset
//	if( curmap->width() * Tile::size <= m_screen->w ) {
//		offsetX =  -m_screen->w / 2 + curmap->width() * Tile::size / 2;
//	} else {
//		offsetX = feetCenterX(hero) - (m_screen->w / 2);
//		if( offsetX < 0 ) offsetX = 0;
//		if( offsetX > mapWidthPix - m_screen->w ) offsetX = mapWidthPix-m_screen->w;
//	}
//
//	if( curmap->height() * Tile::size <= m_screen->h ){
//		offsetY = -m_screen->h / 2 + curmap->height() * Tile::size / 2;
//	} else {
//		offsetY = feetCenterY(hero) - (m_screen->h / 2);
//		if( offsetY < 0 ) offsetY = 0;
//		if( offsetY > mapHeightPix - m_screen->h  ) offsetY = mapHeightPix-m_screen->h;
//	}
    ++m_frameCount;
}

void Gameplay::updateDisplay() {
    //generic background color (delete this crap)
    SDL_FillRect(m_screen, NULL, SDL_MapRGB(m_screen->format, 0,0,0));

    //blit the map
    double screenX = 0.0, screenY = 0.0;

    Map * map = m_currentWorld->getMap();
    map->draw(screenX, screenY, 0);
//    int startX = (int)(absX(0) / Tile::size);
//    int startY = (int)(absY(0) / Tile::size);
//    int endX = (int)(absX(m_screen->w) / Tile::size);
//    int endY = (int)(absY(m_screen->h) / Tile::size);
//
//
//    if( startX < 0) startX = 0;
//    if( startY < 0) startY = 0;
//    if( endX >= curmap->width() ) endX = curmap->width()-1;
//    if( endY >= curmap->height() ) endY = curmap->height()-1;
   
    // TODO: loop through tiles and paint them
//    for(i=0;i<curmap->layerCount();i++){
//        for(y=startY;y<=endY;y++){
//            for(x=startX;x<=endX;x++){
//                //paint this tile
//                if( curmap->squares[y][x][i] >= 0){
//                    destRect.x = screenX(x*Tile::size);
//                    destRect.y = screenY(y*Tile::size);
//                    destRect.w = Tile::size;
//                    destRect.h = Tile::size;
//
//
//                    SDL_BlitSurface( curmap->tiles[curmap->squares[y][x][i]]->bitmap, NULL, m_screen, &destRect);
//                }
//            }
//        }
//
//        if( i == curmap->playerLayer ){
//            //character
//            destRect.x = screenX((int)hero->x);
//            destRect.y = screenY((int)hero->y);
//            destRect.w = hero->curAnimation->frames[hero->curAnimation->curFrame]->w;
//            destRect.h = hero->curAnimation->frames[hero->curAnimation->curFrame]->h;
//
//            SDL_BlitSurface(hero->curAnimation->frames[hero->curAnimation->curFrame], NULL, m_screen, &destRect);
//
//        }
//    }

}
//
//int Gameplay::screenX(int absX){
//	return absX-offsetX;
//}
//
//int Gameplay::screenY(int absY){
//	return absY-offsetY;
//}
//
//int Gameplay::absX(int screenX){
//	return screenX+offsetX;
//}
//
//int Gameplay::absY(int screenY){
//	return screenY+offsetY;
//}
//
//
//
//float Gameplay::feetTop(character* ch){
//	return ch->y + ch->feetOffsetY;
//}
//
//float Gameplay::feetLeft(character* ch){
//	return ch->x + ch->feetOffsetX;
//}
//
//float Gameplay::feetRight(character* ch){
//	return ch->x + ch->feetOffsetX + ch->feetWidth;
//}
//
//float Gameplay::feetBottom(character* ch){
//	return ch->y + ch->feetOffsetY + ch->feetHeight;
//}
//
//float Gameplay::feetCenterX(character* ch){
//	return feetLeft(ch) + ch->feetWidth / 2;
//}
//
//float Gameplay::feetCenterY(character* ch){
//	return feetTop(ch) + ch->feetHeight / 2;
//}
//
//void Gameplay::setCharFeet(character* ch, float newX, float newY){
//	setCharFeetX(ch, newX);
//	setCharFeetY(ch, newY);
//}
//
//void Gameplay::setCharFeetX(character* ch, float newX){
//	ch->x = newX - ch->feetOffsetX;
//}
//
//void Gameplay::setCharFeetY(character* ch, float newY){
//	ch->y = newY - ch->feetOffsetY;
//}
//
//Coord Gameplay::charCoord(character* ch){
//	Coord ret;
//
//	ret.x = (int) floor(feetCenterX(ch) / Tile::size);
//	ret.y = (int) floor(feetCenterY(ch) / Tile::size);
//
//	return ret;
//}
//
//int Gameplay::isValidMove(character* ch, float offX, float offY){
//	int x,y;
//	Coord ft = charCoord(ch);
//
//	//check the rectangle that is the character's feet with nearby obstacles
//	//how about a radius of 1 square.
//
//	for(x=ft.x-1;x<=ft.x+1;x++){
//		for(y=ft.y-1;y<=ft.y+1;y++){
//			if( isObstacle(x,y) && collision( rect(feetLeft(ch)+offX, feetTop(ch)+offY, ch->feetWidth, ch->feetHeight) , rect(x*Tile::size, y*Tile::size, Tile::size, Tile::size) )  )
//				return 0;
//		}
//	}
//
//	return 1;
//}
//
//SDL_Rect Gameplay::rect(int x, int y, int w, int h){
//	SDL_Rect ret;
//
//	ret.x = x;
//	ret.y = y;
//	ret.w = w;
//	ret.h = h;
//
//	return ret;
//}
//
//int Gameplay::collision(SDL_Rect r1, SDL_Rect r2){
//	return ! (r1.x + r1.w < r2.x || r1.x > r2.x + r2.w) && ! (r1.y + r1.h < r2.y || r1.y > r2.y + r2.h);
//}
//
//int Gameplay::isObstacle(int x, int y){
//	int i;
//
//	//out of bounds
//	if( x >= curmap->width || y >= curmap->height || x < 0 || y < 0 ) return 1;
//
//	//check each layer for obstacle tiles
//	for(i=0;i<curmap->numLayers;i++){
//		if(curmap->squares[y][x][i] != -1 && curmap->tiles[curmap->squares[y][x][i]]->obstacle ) return 1;
//	}
//	return 0;
//}
//
//int Gameplay::ptInSquare(int x, int y, int squX, int squY){
//	return x > squX * Tile::size && x < (squX+1) * Tile::size && y > squY * Tile::size && y < (squY+1) * Tile::size;
//}
//
//int Gameplay::keyPressed(SDLKey key){
//	Uint8* keystate = SDL_GetKeyState(NULL);
//	return (int) keystate[key];
//}
