#include "MainWindow.h"

#include "Config.h"
#include "Gameplay.h"

#include <iostream>
#include <cstdlib>

const int MainWindow::c_fps = 48;
const int MainWindow::c_width = 800;
const int MainWindow::c_height = 600;
const int MainWindow::c_colorDepth = 32;
const char * MainWindow::c_caption = "Myth of the Ruby Sword";

MainWindow::MainWindow() :
    m_screen(NULL),
    m_videoModeFlags(SDL_HWSURFACE | SDL_DOUBLEBUF)
{
    // set fullscreen flag
    setFullscreenFlags(Config::instance()->fullscreen());

    // initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "Unable to init SDL: " << SDL_GetError() << std::endl;
        std::exit(1);
    }
    atexit(SDL_Quit);

    // create and display the window
    SDL_WM_SetCaption(c_caption, c_caption);
    // TODO: SDL_WM_SetIcon(SDL_LoadBMP("icon.bmp"), NULL); http://sdl.beuc.net/sdl.wiki/SDL_WM_SetIcon
    m_screen = SDL_SetVideoMode(c_width, c_height, c_colorDepth, m_videoModeFlags);
    if (m_screen == NULL) {
        std::cerr << "Unable to create SDL video surface: "
            << SDL_GetError() << std::endl;
        std::exit(1);
    }

    // Start SDL_mixer
    /* These are the defaults that I got from the tutoral, don't know if we
       have preferred chunk size.
    */
    if ( Mix_OpenAudio( 22050, MIX_DEFAULT_FORMAT, 2, 4096 ) == -1 )
    {
        std::cerr << "Unable to load SDL Mixer: "
                << SDL_GetError() << std::endl;
        std::exit(1);
    }

    // hide mouse cursor
    SDL_ShowCursor(SDL_DISABLE);

    // initialize gameplay
    m_gameplay = new Gameplay(m_screen, c_fps, this);
    if (! m_gameplay->isGood()) {
        std::cerr << "Gameplay did not initialize properly." << std::endl;
        std::exit(1);
    }
}

MainWindow::~MainWindow()
{
    delete m_gameplay;
}

int MainWindow::exec()
{
    m_gameplay->mainLoop();
    return 0;
}

void MainWindow::toggleFullscreen()
{
    setFullscreenFlags(! m_fullscreen);
    m_screen = SDL_SetVideoMode(c_width, c_height, c_colorDepth, m_videoModeFlags);
}

void MainWindow::setFullscreenFlags(bool fullscreen)
{
    m_fullscreen = fullscreen;
    if( m_fullscreen )
        m_videoModeFlags |= SDL_FULLSCREEN;
    else
        m_videoModeFlags &= ~SDL_FULLSCREEN;
}


