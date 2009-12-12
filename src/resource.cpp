#include "SDL.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "resource.h"

#include "debug.h"
#include "gameplay.h"

character* loadResCharacter(const char* filename, const char* charresfile, const char* aniresfile, SDL_Surface* screen){
	int filesize;
	character* ret;
	int* ptr;
	int i, j;
	char* resbuffer;
	int* numProps;
	char* propname;
	int* nextLoc;
	int propValLen;
	
	//printf("about to load character\n");
	
	//load data into memory
	filesize = 0;
	resbuffer = getBufferFromResource( charresfile, filename, &filesize);
	
	//allocate the character
	ret = (character*)malloc( sizeof(character) );
	crashIf(ret == NULL, "Unable to malloc a character", NULL);
	
	/*
		int version
		int numprops
		for each numprops
		  int propnamelength
		  char[propnamelength] propname
		  int propvaluelength
		  <propvalue, depends on propname>
	*/
	ptr = (int*) resbuffer;
	crashIf( *(ptr++) != 1, "This engine can only handle character version 1.", NULL);
	
	
	//default property values
	ret->curAnimation = NULL;
	ret->walking = (animation**) malloc(sizeof(animation*) * 9);
	ret->running = (animation**) malloc(sizeof(animation*) * 9);
	ret->standing = (animation**) malloc(sizeof(animation*) * 9);
	crashIf( ret->walking == NULL || ret->running == NULL || ret->standing == NULL, "Unable to malloc animations for ", filename);
	
	for(i=0;i<9;i++){
		ret->walking[i] = NULL;
		ret->running[i] = NULL;
		ret->standing[i] = NULL;
	}
	ret->feetOffsetX = 0;
	ret->feetOffsetY = 0;
	ret->feetWidth = 32;
	ret->feetHeight = 32;
	ret->speed = 0;
	ret->x = 0;
	ret->y = 0;
	ret->velX = 0;
	ret->velY = 0;
	
	//load properties
	numProps = ptr++;
	//printf("about to load %d properties\n",*numProps);
	for(i=0;i<*numProps;i++){
		propname = readCStr(ptr);
		advanceStrPtr(&ptr);
		
		
		//printf("property %d out of %d is called '%s'\n",i,*numProps, propname);
		propValLen = *ptr++;
		nextLoc = (int*) (((char*) ptr) + propValLen); //calculate next value location
		if( strcmp(propname, "walking") == 0){
			for(j=0;j<9;j++){
				if( *ptr > 0)
					ret->walking[j] = loadResAnimation(readCStr(ptr), aniresfile,  screen);
				advanceStrPtr(&ptr);
			}
		} else if( strcmp(propname, "running") == 0) {
			for(j=0;j<9;j++){
				if( *ptr > 0)
					ret->running[j] = loadResAnimation(readCStr(ptr), aniresfile, screen);
				advanceStrPtr(&ptr);
			}
		} else if( strcmp(propname, "standing") == 0) {
			for(j=0;j<9;j++){
				if( *ptr > 0)
					ret->standing[j] = loadResAnimation(readCStr(ptr), aniresfile, screen);
				advanceStrPtr(&ptr);
			}
		} else if(strcmp(propname, "feetoffsetx") == 0) {
			ret->feetOffsetX = *ptr;
		} else if(strcmp(propname, "feetoffsety") == 0) {
			ret->feetOffsetY = *ptr;
		} else if(strcmp(propname, "feetwidth") == 0) {
			ret->feetWidth = *ptr;
		} else if(strcmp(propname, "feetheight") == 0) {
			ret->feetHeight = *ptr;
		} else if(strcmp(propname, "speed") == 0) {
			ret->speed = *((float*)ptr);
		} else {
			printf("unrecognized character property in %s: %s\n", filename,propname);
		}
		
		ptr = nextLoc;
	}
	
	//printf("successfully loaded character (%d props)\n", *numProps);
	
	free(resbuffer);
	return ret;
}

map* loadResMap(const char* filename, const char* mapresfile, const char* tileresfile, const char* bmpresfile, const char* aniresfile, SDL_Surface* screen){
	//create a map structure from resource file
	char* resbuffer;
	int filesize;
	map* ret;
	int i, x, y;
	int* ptr;
	char* tileName;
	tile* tmptile;
	
	
	//load data into memory
	filesize = 0;
	resbuffer = getBufferFromResource( mapresfile, filename, &filesize);
	
	//allocate the map
	ret = (map*) malloc( sizeof(map) );
	crashIf(ret == NULL, "Unable to malloc a map", NULL);
	
	/*
		int version
		int numtiles
		for each numtiles
		  int tilenamelength
		  char[tilenamelength] tilename
		int mapwidth
		int mapheight
		int numlayers
		int playerlayer
		for each mapheight
		  for each mapwidth
			for each numlayers
			  int tileindex
      */
	ptr = (int*) resbuffer;
	crashIf( *(ptr++) != 1, "Game can only handle map version 1. You probably need to update.", NULL);
	
	ret->numTiles = *(ptr++);
	
	ret->tiles = (tile **) malloc( sizeof(tile) * ret->numTiles);
	crashIf( ret->tiles == NULL, "Unable to malloc tiles for map",NULL);
	
	//printf("about to read tile names. num tiles: %d\n", ret->numTiles );
	for(i=0;i<ret->numTiles;i++){
		//get tile name
		tileName = readCStr(ptr);
		advanceStrPtr(&ptr);
		
		//printf("tile name: %s\n", tileName);
		
		//load that tile from resource
		//printf("about to load tile '%s'\n", tileName);
		tmptile = loadResTile(tileName, tileresfile, aniresfile, bmpresfile,  screen);
		//printf("done.\n");
		ret->tiles[i] = tmptile;
	}
	
	//printf("about to read w,h,nl\n");
	ret->width = *(ptr++);
	ret->height = *(ptr++);
	ret->numLayers = *(ptr++);
	ret->playerLayer = *(ptr++);
	
	//squares
    // this is dumb
	ret->squares = (int***) malloc( sizeof(int**) * ret->height );
	crashIf( ret->squares == NULL, "Unable to allocate column in map", NULL);
	for(y=0;y<ret->height;y++){
		ret->squares[y] = (int**) malloc( sizeof(int*) * ret->width  );
		crashIf(ret->squares[y] == NULL, "Unable to allocate row in map", NULL);
		for(x=0;x<ret->width;x++){
			ret->squares[y][x] = (int*) malloc( sizeof(int) * ret->numLayers );
			crashIf(ret->squares[y][x] == NULL, "Unable to allocate map sqare", NULL);
			for(i=0;i<ret->numLayers;i++){
				ret->squares[y][x][i] = *(ptr++);
			}
		}
	}
	free(resbuffer);
	return ret;
}

char* readCStr(int* buffer ){
	char* ret;
	
	ret = (char*) malloc( (*buffer+1) * sizeof(char) );
	crashIf( ret == NULL, "Unable to malloc string", NULL);
	
	memcpy(ret, buffer+1, *buffer);
	ret[*buffer] = 0; //null byte
	
	
	return ret;
}

void advanceStrPtr(int** ptr){
	*ptr = (int*) (((char*) (*ptr+1)) + **ptr); //move to after string
}

tile* loadResTile(const char* filename, const char* tileresfile, const char* aniresfile, const char* bmpresfile, SDL_Surface* screen){
	tile* ret;
	int filesize;
	char* resbuffer;
	int* ptr;
	int i;
	char* propname;
	int* propValLen;
	int* numProps;
	char* ffile;
	
	filesize = 0;
	resbuffer = getBufferFromResource(tileresfile, filename, &filesize);
	
	ret = (tile*) malloc(sizeof(tile));
	crashIf( ret == NULL, "Unable to malloc tile", NULL);
	
	/*
		int version
		int numprops
		for each numprops
		  int propnamelength
		  char[propnamelength] propname
		  int propvaluelength
		  <propvalue, depends on propname>
	*/
	ptr = (int*) resbuffer;
	crashIf( *(ptr++) != 1, "This engine can only handle tile version 1.", NULL);
	
	
	//default property values
	ret->bitmap = NULL;
	ret->colorKey = 0;
	ret->useColorKey = 0;
	ret->ground = gtNormal;
	ret->obstacle = 0;
	ret->visible = 1;
	ret->portal = NULL;
	ret->ani = NULL;
	
	//load properties
	numProps = ptr++;
	//printf("this tile has %d properties\n", *numProps);
	for(i=0;i<*numProps;i++){
		//printf("prop name length: %d\n",*ptr);
		propname = readCStr(ptr);
		advanceStrPtr(&ptr);
		//printf("property name: %s\n",propname);
		
		
		propValLen = ptr++;
		//printf("property value length is %d\n", *propValLen);
		if( strcmp(propname, "bitmap") == 0){
			ffile = readCStr( propValLen );
			//load bitmap into a SDL_Surface*
			//printf("bitmap is %s, reading that from resource file.\n",bitmap);
			ret->bitmap = loadResBMP(ffile, bmpresfile);
			mfree(ffile);
			//set colorkey
			SDL_SetColorKey( ret->bitmap, SDL_SRCCOLORKEY|SDL_RLEACCEL, ret->colorKey);
		} else if(strcmp(propname, "alpha") == 0) {
			//create colorkey
			ret->useColorKey = 1;
			ret->colorKey = SDL_MapRGB(screen->format , *((char*) ptr), *(((char*) ptr)+1), *(((char*) ptr)+2));
		} else if(strcmp(propname, "obstacle") == 0) {
			ret->obstacle = *((char*) ptr);
		} else if(strcmp(propname, "ground") == 0) {
			ret->ground = *ptr;
		} else if(  strcmp(propname, "animation") == 0   ){
			ffile = readCStr( propValLen );
			ret->ani = loadResAnimation(ffile, aniresfile, screen);
			mfree(ffile);
		} else if( strcmp(propname, "visible") == 0 ){
			ret->visible = *((char*) ptr);
		} else if( strcmp(propname, "portal") == 0 ) {
			
			ret->portal = (mapLink *) malloc(sizeof(mapLink));
			crashIf(ret->portal == NULL, "Unable to malloc memory for portal", NULL);
			
			ret->portal->mapName = readCStr( ptr );
			advanceStrPtr( &ptr );
			ret->portal->x = *(ptr++);
			ret->portal->y = *(ptr++);
		} else {
			printf("unrecognized tile property in %s: %s\n", filename,propname);
		}
		//printf("going to next property\n");
		
		ptr = (int*) (((char*) ptr) + *propValLen); //skip the property value
	}
	
	//printf("returning tile\n");
	free(resbuffer);
	return ret;
}

animation* loadResAnimation(const char* filename, const char* aniresfile, SDL_Surface* screen){
	int filesize;
	char* resbuffer;
	animation* ret;
	int* numFiles;
	int* filestart;
	int* frameFileSize;
	int* filenamesize;
	int i;
	char* ptr;
	SDL_RWops* rw;
	SDL_Surface* temp;
	
	//grab an animation from a resource and return an array of SDL_Surface pointers as well as setting numFrames
	filesize = 0;
	resbuffer = getBufferFromResource( aniresfile, filename, &filesize);
	
	//allocate the animation
	ret = (animation*)malloc(sizeof(animation));
	crashIf( ret == NULL, "couldn't malloc an animation*", NULL);
	
	//read the memory as a resource file
	//read the first int which will tell us how many files are in the animation
	numFiles = (int*)resbuffer;
	ret->numFrames = *numFiles-1;
	
	//get the pointers to the stored files
	filestart = ((int*)resbuffer) + 1;
	
	//read all the bmps into a SDL_Surface
	for(i=0;i<*numFiles;i++){
	
	
		//get the filesize value
		frameFileSize = (int*) (resbuffer+filestart[i]);
		
		//get the size of the filename string
		filenamesize = frameFileSize+1;
		
		//location of the file
		
		ptr = resbuffer+filestart[i]+sizeof(int)*2+*filenamesize;
		if( i == 0 ){
			//descriptor
			
			
			ret->colorKey = SDL_MapRGB(screen->format, (Uint8) ptr[0], (Uint8) ptr[1], (Uint8) ptr[2]  );
			
			ret->numFrames = *( (int*)(ptr+3)  );
			ret->delay = *( ((int*)(ptr+3)) +1  );
			
			
			ret->frames = (SDL_Surface **) malloc( ret->numFrames * sizeof(SDL_Surface *) );
			crashIf(ret->frames == NULL, "couldn't malloc a SDL_Surface**", NULL );
			
		} else if( i-1 < ret->numFrames) {
			//actual frame
			
			
			//get the contents of file, read it into a SDL_Surface
			rw = SDL_RWFromMem( ptr, *frameFileSize);
			temp = SDL_LoadBMP_RW(rw, 1);
		
			crashIf(temp == NULL, "loadResAnimation: Unable to load a bitmap from buffer.", NULL);
		
			//convert image to optimal display format
			ret->frames[i-1] = SDL_DisplayFormat(temp);
			SDL_FreeSurface(temp);
		
			//set color key
			crashIf( SDL_SetColorKey( ret->frames[i-1], SDL_SRCCOLORKEY|SDL_RLEACCEL, ret->colorKey), "unable to set color key", SDL_GetError());
		}
	}
	
	ret->curFrame = 0;
	ret->delayCount = 0;
	free(resbuffer);
	return ret;
}


SDL_Surface* loadResBMP(const char* filename, const char* bmpresfile ){
	//get the bitmap's buffer and size from the resource file
	int filesize;
	char* buffer;
	SDL_RWops* rw;
	SDL_Surface* temp;
	SDL_Surface* image;


	filesize = 0;
	buffer = getBufferFromResource( bmpresfile, filename, &filesize);
	
	//load the buffer into a surface using RWops
	rw = SDL_RWFromMem(buffer, filesize);
	temp = SDL_LoadBMP_RW(rw, 1);
	
	//free mem
	free(buffer);
	
	//crash if fails
	crashIf(temp == NULL, "unable to load resource from resource file", NULL);
	
	//convert the image to optimal display format
	image = SDL_DisplayFormat(temp);
	
	//free temporary surface
	SDL_FreeSurface(temp);
	
	return image;
}

char* getBufferFromOpenResourceFile(FILE* rf, const char* resourceName, int* filesize){
	//remember beginning of file
	long int start;
	int fail;
	int numfiles;
	int* filestart;
	int filenamesize;
	char* buffer;
	int i;
	char* filename;

	start = ftell(rf);
	
	//keep track of success
	fail = 0;
	
	//read the first int which will tell us how many files are in the resource
	fail = fail || fread(&numfiles, sizeof(int), 1, rf) != 1;
	crashIf(fail, "unable to read numfiles from open file.", NULL);
	
	//get the pointers to the stored files
	filestart = (int *) malloc( sizeof(int) * numfiles);
	crashIf(filestart == NULL, "Unable to malloc filestart pointers", NULL);
	fail = fail || fread(filestart, sizeof(int), numfiles, rf) != numfiles;
	
	//loop through the files, looking for resourceName
	filenamesize = 0;
	for(i=0;i<numfiles;i++){
	
		//seek to the location
		fail = fail || fseek(rf, start + filestart[i], SEEK_SET);
		
		//get the filesize value
		fail = fail || fread( filesize, sizeof(int), 1, rf) != 1;
		
		//get the size of the filename string
		fail = fail || fread( &filenamesize, sizeof(int), 1, rf) != 1;
		
		//size the buffer and read the filename
		crashIf(fail, "File I/O error when reading resources", NULL);
		filename = (char *) malloc(filenamesize + 1);
		crashIf(filename == NULL, "unable to malloc filename string", NULL);
		fail = fail || fread( filename, sizeof(char), filenamesize, rf) != filenamesize;
		crashIf(fail, "unable to read filenamesize", NULL);
		
		//string terminator
		filename[filenamesize] = 0;
		
		//compare to resourceName
		if( strcmp(filename, resourceName) == 0){
			//get the contents of file
			buffer = (char*)malloc(*filesize);
			crashIf(buffer == NULL, "unable to malloc buffer for reading file", NULL);
			fail = fail || fread( buffer, 1, *filesize, rf) != *filesize;
			
			free(filename);
			free(filestart);
			fclose(rf);
			crashIf(fail, "Error reading bytes into buffer", NULL);
			return buffer;
		}
		free(filename);
	}
	//don't need these because exit(1) will do it
	//free(filestart);
	//fclose(rf);
	
	fprintf(stderr, "getBufferFromResource: Unable to find %s in resourcefile.\n", resourceName);
	exit(1);
	
}

char* getBufferFromResource(const char* resourceFile, const char* resourceName, int* filesize){
	//try to open the resource file
	FILE* rf = fopen(resourceFile, "rb");
	crashIf( rf == NULL, "error opening resourcefile", NULL);
	
	
	//seek to beginning
	crashIf( fseek(rf, 0, SEEK_SET), "error seeking to beginning of file", NULL );

	
	return getBufferFromOpenResourceFile(rf, resourceName, filesize);
}

void deallocmap(map* m){
	int x,y,i;
	
	//free squares
	for(y=0;y<m->height;y++){
		for(x=0;x<m->width;x++){
			free(m->squares[y][x]);
		}
		free(m->squares[y]);
	}
	free(m->squares);
	
	//free tiles
	for(i=0;i<m->numTiles;i++){
		dealloctile(m->tiles[i]);
	}
	free(m->tiles);
}

void dealloctile(tile* t){
	if( t->bitmap != NULL )
		free(t->bitmap);
	if( t->ani != NULL )
		deallocanimation(t->ani);
	if( t->portal != NULL ){
		free(t->portal->mapName);
		free(t->portal);
	}
}

void deallocanimation(animation* a){
	int i;
	
	for(i=0;i<a->numFrames;i++){
		free(a->frames[i]);
	}
	free(a->frames);
}
