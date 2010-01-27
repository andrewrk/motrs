#include "Gameplay.h"

#include "ResourceManager.h"
#include "Input.h"
#include "Tile.h"
#include "os_config.h"
#include "Debug.h"
#include "Utils.h"

#include <cmath>

#include <iostream>

#ifndef RELEASE
const char * Gameplay::ResourceFilePath = "resources.dat";
#else
const char * Gameplay::ResourceFilePath = RESOURCE_DIR "/resources.dat";
#endif

const int Gameplay::WingDirectionsMap[] = {
    1 << Entity::East | 1 << Entity::South,
    1 << Entity::SouthEast | 1 << Entity::SouthWest,
    1 << Entity::South | 1 << Entity::West,
    1 << Entity::NorthEast | 1 << Entity::SouthEast,
    0,
    1 << Entity::SouthWest | 1 << Entity::NorthWest,
    1 << Entity::North | 1 << Entity::East,
    1 << Entity::NorthWest | 1 << Entity::NorthEast,
    1 << Entity::West | 1 << Entity::North,
};

Gameplay * Gameplay::s_inst = NULL;

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

    return true;
}

void Gameplay::nextFrame() {
    // refresh the input state
    Input::refresh();

    // iterate map set once
    std::vector<Map*> maps;
    maps.reserve(m_loadedMaps.size());
    for (std::set<Map*>::iterator iMap = m_loadedMaps.begin(); iMap != m_loadedMaps.end(); iMap++)
        maps.push_back(*iMap);

    // determin directional input
    double dx = m_player->velocityX();
    double dy = m_player->velocityY();

    switch (m_player->movementMode()) {
    case Entity::Stand:
    case Entity::Walk:
    case Entity::Run: {
            // TODO code duplication
            int north = Input::state(Input::North) ? 1 : 0;
            int east = Input::state(Input::East) ? 1 : 0;
            int south = Input::state(Input::South) ? 1 : 0;
            int west = Input::state(Input::West) ? 1 : 0;
            int input_dx = east - west;
            int input_dy = south - north;
            Entity::Direction direction = (Entity::Direction)((input_dx + 1) + 3 * (input_dy + 1));
            if (direction != Entity::Center)
                m_player->setOrientation(direction);

            bool jump = Input::justPressed(Input::Jump);
            if (jump) {
                m_player->setMovementMode(Entity::JumpUp);
                double jumpingSpeed = 5.0;
                m_player->setAltitudeVelocity(jumpingSpeed);
            } else {
                if (direction == Entity::Center)
                    m_player->setMovementMode(Entity::Stand);
                else
                    m_player->setMovementMode(Entity::Run);
            }
            double speed = 3.3;
            dx = speed * input_dx;
            dy = speed * input_dy;
            if (input_dx != 0 && input_dy != 0) {
                // scale down by sqrt(1/2)
                dx *= Utils::RadHalf;
                dy *= Utils::RadHalf;
            }
        }
        break;
    case Entity::JumpUp: {
            bool keepJumping = Input::state(Input::Jump);
            double maxAltidue = 30.0;
            if (keepJumping)
                keepJumping = m_player->altitude() < maxAltidue;
            if (!keepJumping)
                m_player->setMovementMode(Entity::JumpDown);
            m_player->applyAltitudeVelocity();
        }
        break;
    case Entity::JumpDown: {
            // TODO code duplication
            int north = Input::state(Input::North) ? 1 : 0;
            int east = Input::state(Input::East) ? 1 : 0;
            int south = Input::state(Input::South) ? 1 : 0;
            int west = Input::state(Input::West) ? 1 : 0;
            int input_dx = east - west;
            int input_dy = south - north;
            Entity::Direction direction = (Entity::Direction)((input_dx + 1) + 3 * (input_dy + 1));
            if (direction != Entity::Center)
                m_player->setOrientation(direction);

            double altitudeVelocity = m_player->altitudeVelocity();
            double gravity = 1.0;
            altitudeVelocity -= gravity;
            m_player->setAltitudeVelocity(altitudeVelocity);
            m_player->applyAltitudeVelocity();
            if (m_player->altitude() <= 0.0) {
                // hit the floor
                m_player->setMovementMode(Entity::Stand);
                m_player->setAltitude(0.0);
            }
        }
        break;
    default: Debug::assert(false, "bad movement mode");
    }

    // calculate the desired location
    double x = m_player->centerX() + dx, y = m_player->centerY() + dy;
    double radius = m_player->radius();
    int layer = m_player->layer();

    // resolve collisions
    std::vector<Map::TileAndLocation> tiles;
    for (unsigned int i = 0; i < maps.size(); i++)
        maps[i]->intersectingTiles(tiles, x, y, radius, layer, Tile::ppRail);
    // sort by proximity
    sortByProximity(x, y, tiles);
    // resolve collisions
    for (unsigned int i = 0; i < tiles.size(); i++)
        tiles[i].tile->resolveCircleCollision(tiles[i].x, tiles[i].y, x, y, radius);

    // calculate real dx, dy
    dx = x - m_player->centerX();
    dy = y - m_player->centerY();
    // apply new location
    m_player->setCenter(x, y);
    // store velocity for next fram
    m_player->setVelocity(dx, dy);

    // scroll the screen
    double marginNorth = m_player->centerY() - m_screenY;
    double marginEast = m_screenX + screenWidth() - (m_player->centerX() + m_player->radius());
    double marginSouth = m_screenY + screenHeight() - (m_player->centerY() + m_player->radius());
    double marginWest = m_player->centerX() - m_screenX;

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

void Gameplay::sortByProximity(double x, double y, std::vector<Map::TileAndLocation> & tiles) {
    if (tiles.size() == 0)
        return; // we have to check for this because of stupid unsigned int size;
    for (unsigned int i = 0; i < tiles.size(); i++)
        tiles[i].proximity2 = Utils::distance2(x, y, tiles[i].x, tiles[i].y);
    // selection sort
    for (unsigned int i = 0; i < tiles.size() - 1; i++) {
        unsigned int minIndex = i;
        for (unsigned int j = i + 1; j < tiles.size(); j++) {
            if (tiles[j].proximity2 < tiles[minIndex].proximity2)
                minIndex = j;
        }
        if (minIndex != i) {
            Map::TileAndLocation tmp = tiles[i];
            tiles[i] = tiles[minIndex];
            tiles[minIndex] = tmp;
        }
    }
}

void Gameplay::updateDisplay() {
    //generic background color
    SDL_FillRect(m_screen, NULL, SDL_MapRGB(m_screen->format, 0,0,0));

    //blit the map
    Map * map = m_currentWorld->getMap();
    for (int layer = 0; layer < map->layerCount(); layer++) {
        map->draw(m_screenX, m_screenY, screenWidth(), screenHeight(), layer);
        if (layer == m_player->layer())
            m_player->draw(m_screenX, m_screenY);
    }
}
