#include "SDL.h"
#include "Gameplay.h"
#include "Utils.h"

#include <iostream>
#include <string>
#include <cstdlib>

const int fps = 48;

int main(int argc, char* argv[]) {

    //grab parameters
    bool windowMode = 0;
    for (int i = 1; i < argc; i++)
        windowMode |= std::string(argv[i]).compare("--windowed") == 0;

    // init SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "Unable to init SDL: " << SDL_GetError() << std::endl;
        std::exit(1);
    }
    atexit(SDL_Quit);

    // create screen
    Uint32 vmflags = SDL_HWSURFACE|SDL_DOUBLEBUF;
    // TODO tmp
    windowMode = true;
    if (! windowMode)
        vmflags |= SDL_FULLSCREEN;

    SDL_Surface * screen = SDL_SetVideoMode(800, 600, 32, vmflags);
    if (screen == NULL) {
        std::cerr << "Unable to create SDL video surface: "
            << SDL_GetError() << std::endl;
        std::exit(1);
    }

    Gameplay * gameplay = new Gameplay(screen, fps);
    bool good = gameplay->isGood();
    if (good)
        gameplay->mainLoop();

    delete gameplay;
    return good ? 0 : 1;
}

