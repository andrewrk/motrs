#ifndef RESOURCE_H
#define RESOURCE_H


#define mfree(x) free(x); x = NULL

enum GroundType {
	gtNormal, gtWater, gtMud
};

enum AnimationDirection {
	adNorthWest, adWest, adSouthWest, adNorth, adCenter, adSouth, adNorthEast, adEast, adSouthEast
};

typedef struct {
	SDL_Surface** frames;
	Uint32 colorKey;
	
	int numFrames;
	int curFrame;
	
	int delay;
	int delayCount;
	
} animation;

typedef struct {
	char* mapName;
	int x;
	int y;
} mapLink;

typedef struct {
	SDL_Surface* bitmap; //NULL if animated or invisible
	animation* ani; //NULL if bitmap or invisible
	char visible; //boolean
	Uint32 colorKey;
	char useColorKey; //boolean
	char obstacle; //boolean
	int ground;
	mapLink* portal;
} tile;

typedef struct {
	animation* curAnimation;
	int facing;
	
	animation** walking;
	animation** running;
	animation** standing;
	
	int feetOffsetX;
	int feetOffsetY;
	int feetWidth;
	int feetHeight;
	
	float speed;
	
	float x;
	float y;
	float velX;
	float velY;
} character;

typedef struct {
	int numTiles;
	tile** tiles; //1d array of tile*

	int width;
	int height;
	int numLayers;
	int*** squares; //3d array of int - [y][x][i]

	int playerLayer;
} map;

char* readCStr(int* buffer );
void advanceStrPtr(int** ptr);

char* getBufferFromOpenResourceFile(FILE* rf, const char* resourceName, int* filesize);
char* getBufferFromResource(const char* resourceFile, const char* resourceName, int* filesize);

SDL_Surface* loadResBMP(const char* filename, const char* bmpresfile );
tile* loadResTile(const char* filename, const char* tileresfile,
    const char* aniresfile, const char* bmpresfile, SDL_Surface* screen);
map* loadResMap(const char* filename, const char* mapresfile,
    const char* tileresfile, const char* bmpresfile,
    const char* aniresfile, SDL_Surface* screen);
animation* loadResAnimation(const char* filename, const char* aniresfile,
    SDL_Surface* screen);
character* loadResCharacter(const char* filename, const char* charresfile,
    const char* aniresfile, SDL_Surface* screen);


void deallocanimation(animation* a);
void dealloctile(tile* t);
void deallocmap(map* m);

#endif
