#include "SDL.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gameplay.h"

#define FPS 48


int main(int argc, char* argv[]) {
    const Uint32 interval = 1000 / FPS; //frames per second -> miliseconds

    //grab parameters
    int windowMode = 0;
    int testMapMode = 0;
    char* mapToTest = NULL;
    char* charToTest = NULL;
    int testStartX = -1, testStartY = -1;
    for (int i = 1; i < argc; i++) {
        windowMode  |= strcmp("--windowed",argv[i]) == 0;
        testMapMode |= strcmp("--testmap", argv[i]) == 0;
        if (strcmp("--testmap",argv[i]) == 0 && ++i < argc) {
            mapToTest = argv[i];
        } else if (strcmp("--char", argv[i]) == 0 && ++i < argc) {
            charToTest = argv[i];
        } else if (strcmp("--startx", argv[i]) == 0 && ++i < argc) {
            testStartX = atoi(argv[i]);
        } else if (strcmp("--starty", argv[i]) == 0 && ++i < argc) {
            testStartY = atoi(argv[i]);
        }
    }

    //init SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "Unable to init SDL: %s\n", SDL_GetError());
        exit(1);
    }
    atexit(SDL_Quit);


    //create screen
    Uint32 vmflags = SDL_HWSURFACE|SDL_DOUBLEBUF;
    if (!windowMode)
        vmflags |= SDL_FULLSCREEN;

    SDL_Surface* screen = SDL_SetVideoMode(800, 600, 16, vmflags);
    if (screen == NULL) {
        fprintf(stderr, "Unable to set %d x %d x %d video: %s\n", 800, 600, 16, SDL_GetError());
        exit(1);
    }

    //init gameplay
    if (testMapMode)
        initMapTest(screen, mapToTest, testStartX, testStartY, charToTest);
    else
        initGameplay(screen);

    //main loop
    Uint32 next_time = 0;
    while (1) {
        processEvents(); //input

        //set up an interval
        Uint32 now = SDL_GetTicks();
        if (now >= next_time) {
            SDL_Flip(screen); //show the frame that is already drawn
            nextFrame(); //main gameplay loop
            updateDisplay(); //begin drawing next frame

            next_time = now + interval;
        }

        SDL_Delay(1); //give up the cpu
    }

    return 0;
}

