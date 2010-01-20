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

Gameplay::Gameplay(SDL_Surface * screen, int fps) :
    m_good(true),
    m_screen(screen),
    m_fps(fps),
    m_interval(1000/fps), //frames per second -> miliseconds
    m_frameCount(0),
    m_screenX(0.0), m_screenY(0.0),
    m_universe(NULL),
    m_currentWorld(NULL),
    m_loadedMaps(),
    m_player(NULL)
{
    Debug::assert(s_inst == NULL, "only one Gameplay allowed");
    s_inst = this;
    m_universe = ResourceManager::loadUniverse(ResourceFilePath, "main.universe");
    if (!(m_universe != NULL && m_universe->isGood())) {
        m_good = false;
        return;
    }
    if (m_universe->worldCount() == 0) {
        std::cerr << "no worlds" << std::endl;
        m_good = false;
        return;
    }
    m_currentWorld = m_universe->startWorld();
    m_loadedMaps.insert(m_currentWorld->getMap());
    m_player = m_universe->player();
}

Gameplay::~Gameplay() {
    if (m_universe != NULL)
        delete m_universe;
    s_inst = NULL;
}

void Gameplay::mainLoop() {
    Uint32 next_time = 0;
    while (true) {
        //input
        if (!processEvents())
            return;

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

bool Gameplay::processEvents() {
    SDL_Event event;
    while(SDL_PollEvent(&event)){
        switch(event.type){
            case SDL_KEYDOWN:
                // Handle Alt+F4 for windows
                if (event.key.keysym.sym == SDLK_F4 && (event.key.keysym.mod & KMOD_ALT))
                    return false;
                break;
            case SDL_QUIT:
                return false;
        }
    }
    Input::refresh();

    return true;
}


void Gameplay::nextFrame() {

    // determin directional input
    int north = Input::state(Input::North) ? 1 : 0;
    int east = Input::state(Input::East) ? 1 : 0;
    int south = Input::state(Input::South) ? 1 : 0;
    int west = Input::state(Input::West) ? 1 : 0;
    int input_dx = east - west;
    int input_dy = south - north;
    Entity::Direction direction = (Entity::Direction)((input_dx + 1) * 3 + (input_dy + 1));
    if (direction == Entity::Center) {
        m_player->setMovementMode(Entity::Stand);
    } else {
        m_player->setMovementMode(Entity::Run);
        m_player->orient(direction);
    }

    // apply the desired velocity
    double speed = 3.0;
    double dx = speed * input_dx;
    double dy = speed * input_dy;
    m_player->move(dx, dy);

    // resolve collisions
    //  collect intersecting tiles
    double left = m_player->feetX(), top = m_player->feetY();
    double width = m_player->feetWidth(), height = m_player->feetHeight();
    int layer = m_player->feetLayer();
    std::vector<Map::TileAndLocation> tiles;
    tiles.reserve(30);
    for (std::set<Map*>::iterator iMap = m_loadedMaps.begin(); iMap != m_loadedMaps.end(); iMap++)
        (*iMap)->intersectingTiles(tiles, left, top, width, height, layer);
    //  ask them where to go

    // scroll the screen
    double marginNorth = m_player->feetY() - m_screenY;
    double marginEast =  m_screenX + screenWidth() - (m_player->feetX() + m_player->feetWidth());
    double marginSouth =  m_screenY + screenHeight() - (m_player->feetY() + m_player->feetHeight());
    double marginWest = m_player->feetX() - m_screenX;

    if (marginNorth < minMarginNorth())
        m_screenY -= minMarginNorth() - marginNorth;
    else if (marginSouth < minMarginSouth())
        m_screenY += minMarginSouth() - marginSouth;
    if (marginWest < minMarginWest())
        m_screenX -= minMarginWest() - marginWest;
    else if (marginEast < minMarginEast())
        m_screenX += minMarginEast() - marginEast;

    m_frameCount++;
}

void Gameplay::updateDisplay() {
    //generic background color (delete this crap)
    SDL_FillRect(m_screen, NULL, SDL_MapRGB(m_screen->format, 0,0,0));

    //blit the map
    Map * map = m_currentWorld->getMap();
    map->draw(m_screenX, m_screenY, 0);

    m_player->draw(m_screenX, m_screenY);
}
