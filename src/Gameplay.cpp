#include "Gameplay.h"

#include "ResourceManager.h"
#include "Input.h"
#include "Tile.h"
#include "os_config.h"
#include "Debug.h"
#include "Utils.h"
#include "MainWindow.h"

#include <cmath>

#include <iostream>

#ifndef RELEASE
const char * Gameplay::ResourceFilePath = "resources.dat";
#else
const char * Gameplay::ResourceFilePath = RESOURCE_DIR "/resources.dat";
#endif

Gameplay * Gameplay::s_inst = NULL;

Gameplay::Gameplay(SDL_Surface * screen, int fps, MainWindow * owner) :
    m_good(true),
    m_screen(screen),
    m_fps(fps),
    m_interval(1000/fps), //frames per second -> miliseconds
    m_frameCount(0),
    m_screenX(0.0), m_screenY(0.0),
    m_universe(NULL),
    m_currentWorld(NULL),
    m_loadedMaps(),
    m_loadedMapsCache(),
    m_entities(),
    m_player(NULL),
    m_window(owner)
{
    assert(s_inst == NULL);
    s_inst = this;

    // initialize gameplay
    m_universe = ResourceManager::loadUniverse(ResourceFilePath, "main.universe");
    if (m_universe == NULL) {
        m_good = false;
        return;
    }
    if (m_universe->worldCount() == 0) {
        std::cerr << "no worlds" << std::endl;
        m_good = false;
        return;
    }
    m_currentWorld = m_universe->startWorld();

    std::vector<Map*>* allMaps = m_currentWorld->maps();
    for (unsigned int i = 0; i < allMaps->size(); i++)
        m_loadedMaps.insert((*allMaps)[i]);

    m_player = m_universe->player();

    Input::init();
}

Gameplay::~Gameplay() {
    if (m_universe != NULL)
        delete m_universe;
    s_inst = NULL;
}

void Gameplay::mainLoop() {
    Uint32 next_time = 0;
    while (true) {
        // input
        if (!processEvents())
            return;

        // set up an interval
        Uint32 now = SDL_GetTicks();
        if (now >= next_time) {
            // show the frame that is already drawn
            SDL_Flip(m_screen);

            // perform calculations
            nextFrame();

            // draw next frame
            updateDisplay();

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
                else if (event.key.keysym.sym == SDLK_RETURN && (event.key.keysym.mod & KMOD_ALT) )
                    m_window->toggleFullscreen();
                break;
            case SDL_QUIT:
                return false;
        }
    }

    return true;
}

void Gameplay::nextFrame() {
    // cache loaded maps
    m_loadedMapsCache.clear();
    for (std::set<Map*>::iterator iMap = m_loadedMaps.begin(); iMap != m_loadedMaps.end(); iMap++)
        m_loadedMapsCache.push_back(*iMap);
    // cache loaded entities
    m_entities.clear();
    m_entities.push_back(m_player);
    for (unsigned int i = 0; i < m_loadedMapsCache.size(); i++) {
        std::vector<Entity*>* mapEntities = m_loadedMapsCache[i]->entities();
        for (unsigned int j = 0; j < mapEntities->size(); j++)
            m_entities.push_back((*mapEntities)[j]);
    }


    // refresh the input state
    Input::refresh();

    for (unsigned int i = 0; i < m_entities.size(); i++)
        applyInput(m_entities[i], m_entities[i] == m_player);
    for (unsigned int i = 0; i < m_entities.size() - 1; i++)
        for (unsigned int j = i + 1; j < m_entities.size(); j++)
            m_entities[i]->resolveCollision(m_entities[j]);
    for (unsigned int i = 0; i < m_entities.size(); i++)
        resolveWithWorld(m_entities[i]);

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

void Gameplay::applyInput(Entity * entity, bool takesInput) {
    // determin directional input
    double dx = entity->velocityX();
    double dy = entity->velocityY();

    bool canChangeDirections = false, canMoveAround = false;
    bool canStartJump = false;
    bool canKeepJumping = false;
    bool canSwingSword = false;
    bool isFalling = false;
    bool affectedByFriction = false;

    switch (entity->movementMode()) {
    case Entity::Stand:
    case Entity::Walk:
    case Entity::Run:
        canChangeDirections = takesInput;
        canStartJump = takesInput;
        canSwingSword = takesInput;
        canMoveAround = takesInput;
        affectedByFriction = true;
        break;
    case Entity::JumpUp:
        canKeepJumping = takesInput;
        canSwingSword = takesInput;
        break;
    case Entity::Falling:
        canChangeDirections = takesInput;
        isFalling = true;
        canSwingSword = takesInput;
        break;
    default: assert(false);
    }

    if (canChangeDirections) {
        int north = Input::state(Input::North) ? 1 : 0;
        int east = Input::state(Input::East) ? 1 : 0;
        int south = Input::state(Input::South) ? 1 : 0;
        int west = Input::state(Input::West) ? 1 : 0;
        int input_dx = east - west;
        int input_dy = south - north;
        Entity::Direction direction = (Entity::Direction)((input_dx + 1) + 3 * (input_dy + 1));
        if (direction != Entity::Center)
            entity->setOrientation(direction);

        if (canMoveAround) {
            if (direction == Entity::Center)
                entity->setMovementMode(Entity::Stand);
            else
                entity->setMovementMode(Entity::Run);
            double speed = entity->speed();
            double accelX = speed * input_dx;
            double accelY = speed * input_dy;
            if (input_dx != 0 && input_dy != 0) {
                // for diagonal motion, scale both axes down by sqrt(1/2)
                accelX *= Utils::RadHalf;
                accelY *= Utils::RadHalf;
            }
            dx += accelX;
            dy += accelY;
        }
    }

    if (canStartJump) {
        bool jump = Input::justPressed(Input::Jump);
        if (jump) {
            entity->setMovementMode(Entity::JumpUp);
            double jumpingSpeed = 5.0;
            entity->setAltitudeVelocity(jumpingSpeed);
        }
    }

    if (canKeepJumping) {
        bool keepJumping = Input::state(Input::Jump);
        double maxAltidue = 30.0;
        if (keepJumping)
            keepJumping = entity->altitude() < maxAltidue;
        if (!keepJumping)
            entity->setMovementMode(Entity::Falling);
        entity->applyAltitudeVelocity();
    }

    if (isFalling) {
        double altitudeVelocity = entity->altitudeVelocity();
        double gravity = 0.3;
        altitudeVelocity -= gravity;
        // TODO: terminal velocity
        entity->setAltitudeVelocity(altitudeVelocity);
        entity->applyAltitudeVelocity();
        if (entity->altitude() <= 0.0) {
            // hit the floor
            entity->setMovementMode(Entity::Stand);
            entity->setAltitude(0.0);
        }
    }

    if (affectedByFriction) {
        double friction = 0.6;
        dx *= friction;
        dy *= friction;
    }
    switch (entity->currentSequence()) {
    case Entity::None:
        if (canSwingSword) {
            bool swingSword = Input::justPressed(Input::Attack_1);
            if (swingSword) {
                entity->setCurrentSequence(Entity::Sword);
                entity->resetSequencePosition();
            }
        }
        break;
    case Entity::Sword:
        {
            int sequencePosition = entity->incrementSequencePosition();
            if (sequencePosition >= 10)
                entity->setCurrentSequence(Entity::None);
            break;
        }
    default: assert(false);
    }
    entity->setVelocity(dx, dy);
}

void Gameplay::resolveWithWorld(Entity * entity) {
    // calculate the desired location
    double dx = entity->velocityX(), dy = entity->velocityY();
    double x = entity->centerX() + dx, y = entity->centerY() + dy;
    double radius = entity->radius();
    int layer = entity->layer();

    // resolve collisions
    std::vector<Map::TileAndLocation> tiles;
    for (unsigned int i = 0; i < m_loadedMapsCache.size(); i++)
        m_loadedMapsCache[i]->intersectingTiles(tiles, x, y, radius, layer, Tile::ppRail);
    // sort by proximity
    sortByProximity(x, y, tiles);
    // resolve collisions
    for (unsigned int i = 0; i < tiles.size(); i++)
        tiles[i].tile->resolveCircleCollision(tiles[i].x, tiles[i].y, x, y, radius);

    // calculate real dx, dy
    dx = x - entity->centerX();
    dy = y - entity->centerY();
    // apply new location
    entity->setCenter(x, y);
    // store velocity for next frame
    entity->setVelocity(dx, dy);
}

void Gameplay::sortByProximity(double x, double y, std::vector<Map::TileAndLocation> & tiles) {
    // TODO: code duplication
    if (tiles.size() == 0)
        return; // we have to check for this because of stupid unsigned int size;
    for (unsigned int i = 0; i < tiles.size(); i++)
        tiles[i].proximity2 = Utils::distance2(x, y, tiles[i].x + Tile::size / 2.0, tiles[i].y + Tile::size / 2.0);
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

void Gameplay::sortByCenterY(std::vector<Entity *> & entities) {
    // TODO: code duplication
    if (entities.size() == 0)
        return; // we have to check for this because of stupid unsigned int size;
    // selection sort
    for (unsigned int i = 0; i < entities.size() - 1; i++) {
        unsigned int minIndex = i;
        for (unsigned int j = i + 1; j < entities.size(); j++) {
            if (entities[j]->centerY() < entities[minIndex]->centerY())
                minIndex = j;
        }
        if (minIndex != i) {
            Entity * tmp = entities[i];
            entities[i] = entities[minIndex];
            entities[minIndex] = tmp;
        }
    }
}

void Gameplay::updateDisplay() {
    //generic background color
    SDL_FillRect(m_screen, NULL, SDL_MapRGB(m_screen->format, 0,0,0));

    // find layer count
    int layerCount = 0;
    for (unsigned int i = 0; i < m_loadedMapsCache.size(); i++)
        layerCount = Utils::max(layerCount, m_loadedMapsCache[i]->layerCount());
    for (int layer = 0; layer < layerCount; layer++) {
        for (unsigned int i = 0; i < m_loadedMapsCache.size(); i++) {
            Map * map = m_loadedMapsCache[i];
            if (layer < map->layerCount())
                map->draw(m_screenX, m_screenY, screenWidth(), screenHeight(), layer);
        }
        // TODO: cache entities on each layer
        std::vector<Entity *> entities;
        for (unsigned int i = 0; i < m_entities.size(); i++)
            if (m_entities[i]->layer() == layer)
                entities.push_back(m_entities[i]);
        sortByCenterY(entities);
        for (unsigned int i = 0; i < entities.size(); i++)
            entities[i]->draw(m_screenX, m_screenY);
    }
}
