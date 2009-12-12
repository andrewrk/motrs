#include "SDL.h"
#include <stdio.h>
#include <math.h>

#include "gameplay.h"



#include "resource.h"
#include "debug.h"

#define Min(x,y) ( ((x) > (y)) ? (y) : (x))
#define Max(x,y) ( ((x) > (y)) ? (x) : (y))



//constants
const int squareWidth = 32;
const int squareHeight = 32;


//gameplay data
SDL_Surface* screen;
map* curmap;
character* hero;

int mapWidthPix, mapHeightPix;
int offsetX, offsetY;
Coord prevHeroLoc;

//surfaces to draw
SDL_Surface* dash;


char aniResLoc[] = "resource/ani.dat";
char bmpResLoc[] = "resource/bmp.dat";
char tileResLoc[]= "resource/tile.dat";
char mapResLoc[] = "resource/map.dat";
char charResLoc[]= "resource/char.dat";

void initGameplay(SDL_Surface* screen){
	printf("uhhh, yeah, we have yet to program the actual game. Try using --testmap <mapname>\n");
	exit(1);
	
}

void enterMap(char* mapFile, int startX, int startY){
	//deallocate the old map
	if( curmap != NULL)
		deallocmap(curmap);
				
	//load map
	curmap = loadResMap(mapFile, mapResLoc, tileResLoc, bmpResLoc, aniResLoc, screen);
	
	if( startX == -1 ) startX = (int)(curmap->width / 2);
	if( startY == -1 ) startY = curmap->height-1;
	setCharFeet(hero, startX * squareWidth+1, startY * squareHeight+1);
	hero->velX = 0;
	hero->velY = 0;	
	
	//compute some map info
	mapWidthPix = curmap->width * squareWidth;
	mapHeightPix= curmap->height * squareHeight;
	prevHeroLoc = charCoord(hero);
}

void initMapTest(SDL_Surface* playScreen, char* mapToTest, int startX, int startY, char* charToTest) {
	//remember screen we're playing on
	screen = playScreen;
	curmap = NULL;
	
	//load hero
	hero = loadResCharacter(charToTest == NULL ? "hero.char" : charToTest, charResLoc, aniResLoc, screen);

	//init variables
	
	hero->facing = adSouth;
	hero->curAnimation = hero->standing[hero->facing];
	
	//load dashboard
	dash = loadResBMP("dash.bmp", bmpResLoc);
	crashIf( dash == NULL, "Couldn't load dash.bmp", SDL_GetError());
	crashIf( SDL_SetColorKey(dash, SDL_SRCCOLORKEY|SDL_RLEACCEL, SDL_MapRGB(screen->format, 255,0,255)), "unable to set colorkey", SDL_GetError() );
	
	enterMap(mapToTest, startX, startY);
}

void updateDisplay() {
	int x,y,i;
	SDL_Rect destRect;
	//Uint32 lineColor;
	//SDL_Rect line;
	
	//generic background color (delete this crap)
	SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0,0,0));
	
	//blit the map
	int startX = (int)(absX(0) / squareWidth);
	int startY = (int)(absY(0) / squareHeight);
	int endX = (int)(absX(screen->w) / squareWidth);
	int endY = (int)(absY(screen->h) / squareHeight);
	
	
	if( startX < 0) startX = 0;
	if( startY < 0) startY = 0;
	if( endX >= curmap->width ) endX = curmap->width-1;
	if( endY >= curmap->height) endY = curmap->height-1;
   
  for(i=0;i<curmap->numLayers;i++){
   for(y=startY;y<=endY;y++){
   	for(x=startX;x<=endX;x++){
   		//paint this tile
   			if( curmap->squares[y][x][i] >= 0){
					destRect.x = screenX(x*squareWidth);
					destRect.y = screenY(y*squareHeight);
					destRect.w = squareWidth;
					destRect.h = squareHeight;
					
					
					SDL_BlitSurface( curmap->tiles[curmap->squares[y][x][i]]->bitmap, NULL, screen, &destRect);
				}
   		}
   	}
   	
		if( i == curmap->playerLayer ){
			//character
			destRect.x = screenX((int)hero->x);
			destRect.y = screenY((int)hero->y);
			destRect.w = hero->curAnimation->frames[hero->curAnimation->curFrame]->w;
			destRect.h = hero->curAnimation->frames[hero->curAnimation->curFrame]->h;
			
			SDL_BlitSurface(hero->curAnimation->frames[hero->curAnimation->curFrame], NULL, screen, &destRect);
	
		}
	}
  
	//dashboard
	SDL_BlitSurface(dash, NULL, screen, NULL);
}

int screenX(int absX){
	return absX-offsetX;
}

int screenY(int absY){
	return absY-offsetY;
}

int absX(int screenX){
	return screenX+offsetX;
}

int absY(int screenY){
	return screenY+offsetY;
}


void nextFrame () {
	//which keys are down
	Uint8* keystate;
	int dirX, dirY;
	float angle;
	Coord heroLoc;
	int i;
	int slow;
	
	keystate = SDL_GetKeyState(NULL);
	
	//physics
	
	//which direction to go
	dirX = 0;
	dirY = 0;
	if( keystate[SDLK_LEFT]  || keystate[SDLK_a] ) dirX--;
	if( keystate[SDLK_RIGHT] || keystate[SDLK_d] ) dirX++;
	if( keystate[SDLK_UP]    || keystate[SDLK_w] ) dirY--;
	if( keystate[SDLK_DOWN]  || keystate[SDLK_s] ) dirY++;
	
	if( dirX == 0 && dirY == 0){
		//stand still
		hero->velX = 0;
		hero->velY = 0;
		
		hero->curAnimation = hero->standing[hero->facing];
	} else {
		//run in a direction
		angle = atan2(dirY,dirX);
		hero->velX = hero->speed * cos(angle);
		hero->velY = hero->speed * sin(angle);
		
		hero->facing = (dirX+1)*3 + (dirY+1);
		hero->curAnimation = hero->running[hero->facing];
	}
	
	//step to next frame in animation
	if( ++hero->curAnimation->delayCount > hero->curAnimation->delay ) {
		if( ++hero->curAnimation->curFrame >= hero->curAnimation->numFrames ) hero->curAnimation->curFrame = 0;
		hero->curAnimation->delayCount = 0;
	}
	
	
	//momentum	
	if( isValidMove( hero, hero->velX, 0 )  )
		hero->x += hero->velX;
	else {
		slow = hero->velX / abs(hero->velX);
		for(i=1;i<=abs(hero->velX) && isValidMove( hero, slow, 0 );i++)
			hero->x += slow;
	}

	if( isValidMove( hero, 0, hero->velY )  )
		hero->y += hero->velY;
	else {
		slow = hero->velY / abs(hero->velY);
		for(i=1;i<=abs(hero->velY) && isValidMove( hero, 0, slow );i++)
			hero->y += slow;
	}
	
	//special tiles
	heroLoc = charCoord(hero);
	
	if( heroLoc.x != prevHeroLoc.x || heroLoc.y != prevHeroLoc.y  )
		enterSquare(hero, &heroLoc);
	
	
	//calculate screen offset
	if( curmap->width * squareWidth <= screen->w ) {
		offsetX =  -screen->w / 2 + curmap->width * squareWidth / 2;
	} else {
		offsetX = feetCenterX(hero) - (screen->w / 2);
		if( offsetX < 0 ) offsetX = 0;
		if( offsetX > mapWidthPix - screen->w ) offsetX = mapWidthPix-screen->w;
	}
	
	if( curmap->height * squareHeight <= screen->h ){
		offsetY = -screen->h / 2 + curmap->height * squareHeight / 2;
	} else {
		offsetY = feetCenterY(hero) - (screen->h / 2);
		if( offsetY < 0 ) offsetY = 0;
		if( offsetY > mapHeightPix - screen->h  ) offsetY = mapHeightPix-screen->h;
	}
}

void enterSquare(character* ch, Coord* loc){
	int i;
	int* squ;
	tile* ti;
	char* newMapName;
	
	for(i=0;i<curmap->numLayers;i++){
		squ = curmap->squares[loc->y][loc->x] + i;
		if( *squ != -1 ){
			ti = curmap->tiles[*squ];
			if( ti->portal != NULL ){
				//when the map gets deallocated, the string that we need for the new map will be erased
				newMapName = (char *)malloc( sizeof(char) * (strlen(ti->portal->mapName)+1) );
				crashIf(newMapName == NULL, "Unable to malloc map name", NULL);
				strcpy(newMapName, ti->portal->mapName);
				
				//enter the new map
				enterMap(newMapName, ti->portal->x, ti->portal->y);
				
				mfree(newMapName);
			}
		}
	}
}

void processEvents() {
	//handle all events
	SDL_Event event;
	while(SDL_PollEvent(&event)){
		switch(event.type){
			case SDL_KEYDOWN:
				switch( event.key.keysym.sym ){
					case SDLK_ESCAPE:
						exit(0);
					default:
						//do nothing
						break;
				}
				break;
			case SDL_QUIT:
				exit(0);
		}
	}
}

float feetTop(character* ch){
	return ch->y + ch->feetOffsetY;
}

float feetLeft(character* ch){
	return ch->x + ch->feetOffsetX;
}

float feetRight(character* ch){
	return ch->x + ch->feetOffsetX + ch->feetWidth;
}

float feetBottom(character* ch){
	return ch->y + ch->feetOffsetY + ch->feetHeight;
}

float feetCenterX(character* ch){
	return feetLeft(ch) + ch->feetWidth / 2;
}

float feetCenterY(character* ch){
	return feetTop(ch) + ch->feetHeight / 2;
}

void setCharFeet(character* ch, float newX, float newY){
	setCharFeetX(ch, newX);
	setCharFeetY(ch, newY);
}

void setCharFeetX(character* ch, float newX){
	ch->x = newX - ch->feetOffsetX;
}

void setCharFeetY(character* ch, float newY){
	ch->y = newY - ch->feetOffsetY;
}

/*int isValidMove(character* ch, int offX, int offY){
	int x,y;
	int squX, squY;
	
	//can't pass through obstacles
	for(x=feetLeft(ch)+offX;x<=feetRight(ch)+offX;x+=collisionMargin){
		for(y=feetTop(ch)+offY;y<=feetBottom(ch)+offY;y+=collisionMargin){
			squX = (int) floor( (float)x / (float)squareWidth);
			squY = (int) floor( (float)y / (float)squareHeight);
			
			if( ptInSquare(x,y,squX,squY) && isObstacle(squX,squY) ){
				//can't walk here
				return 0;
			}
		}
	}
	return 1;
}*/

Coord charCoord(character* ch){
	Coord ret;
	
	ret.x = (int) floor(feetCenterX(ch) / squareWidth);
	ret.y = (int) floor(feetCenterY(ch) / squareHeight);
	
	return ret;
}

int isValidMove(character* ch, float offX, float offY){
	int x,y;
	Coord ft = charCoord(ch);
	
	//check the rectangle that is the character's feet with nearby obstacles
	//how about a radius of 1 square.
	
	for(x=ft.x-1;x<=ft.x+1;x++){
		for(y=ft.y-1;y<=ft.y+1;y++){
			if( isObstacle(x,y) && collision( rect(feetLeft(ch)+offX, feetTop(ch)+offY, ch->feetWidth, ch->feetHeight) , rect(x*squareWidth, y*squareHeight, squareWidth, squareHeight) )  )
				return 0;
		}
	}
	
	return 1;
}

SDL_Rect rect(int x, int y, int w, int h){
	SDL_Rect ret;
	
	ret.x = x;
	ret.y = y;
	ret.w = w;
	ret.h = h;
	
	return ret;
}

int collision(SDL_Rect r1, SDL_Rect r2){
	return ! (r1.x + r1.w < r2.x || r1.x > r2.x + r2.w) && ! (r1.y + r1.h < r2.y || r1.y > r2.y + r2.h);
}

int isObstacle(int x, int y){
	int i;

	//out of bounds
	if( x >= curmap->width || y >= curmap->height || x < 0 || y < 0 ) return 1;
	
	//check each layer for obstacle tiles
	for(i=0;i<curmap->numLayers;i++){
		if(curmap->squares[y][x][i] != -1 && curmap->tiles[curmap->squares[y][x][i]]->obstacle ) return 1;
	}
	return 0;
}

int ptInSquare(int x, int y, int squX, int squY){
	return x > squX * squareWidth && x < (squX+1) * squareWidth && y > squY * squareWidth && y < (squY+1) * squareHeight;
}

int keyPressed(SDLKey key){
	Uint8* keystate = SDL_GetKeyState(NULL);
	return (int) keystate[key];
}
