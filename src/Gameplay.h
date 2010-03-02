#ifndef _GAMEPLAY_H_
#define _GAMEPLAY_H_

#include <SFML/Window.hpp>

#include "Universe.h"
#include "Entity.h"
#include "ResourceFile.h"
#include "Debug.h"
#include "Map.h"
#include "Input.h"

#include <set>

class MainWindow;

class Gameplay
{
public:
    static inline Gameplay * instance();

    Gameplay(MainWindow * owner);
    ~Gameplay();

    void mainLoop();
    bool isGood();

    inline long long int frameCount();
    int fps();
    inline sf::RenderWindow * screen();

    inline double screenWidth();
    inline double screenHeight();

    void updateDisplay();
    void nextFrame();
private: //variables
    static const char * ResourceFilePath;
    static Gameplay * s_inst;

    static void sortByProximity(double x, double y, std::vector<Map::TileAndLocation> & tiles);
    static void sortByCenterY(std::vector<Entity *> & entities);


    bool m_good;
    sf::RenderWindow * m_screen;
    int m_interval;
    long long int m_frameCount;

    double m_screenX, m_screenY;

    Universe * m_universe;

    World * m_currentWorld;
    std::set<Map*> m_loadedMaps;
    std::vector<Map*> m_loadedMapsCache;
    std::vector<Entity*> m_entities;
    Entity * m_player;

    MainWindow * m_window;
    Input * m_input;

private: //methods
    void applyInput(Entity * entity, bool takesInput);
    void resolveWithWorld(Entity * entity);

    double minMarginNorth() { return 250.0; }
    double minMarginEast() { return 350.0; }
    double minMarginSouth() { return 250.0; }
    double minMarginWest() { return 350.0; }

};

inline Gameplay * Gameplay::instance()
{
    assert(s_inst != NULL);
    return s_inst;
}

inline long long int Gameplay::frameCount()
{
    return m_frameCount;
}

inline sf::RenderWindow * Gameplay::screen()
{
    return m_screen;
}

inline double Gameplay::screenWidth()
{
    return 800.0;
}

inline double Gameplay::screenHeight()
{
    return 600.0;
}

#endif

