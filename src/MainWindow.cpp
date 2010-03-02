#include "MainWindow.h"

#include "Config.h"
#include "Gameplay.h"

#include <iostream>
#include <cstdlib>

const int MainWindow::c_width = 800;
const int MainWindow::c_height = 600;
const int MainWindow::c_colorDepth = 32;
const char * MainWindow::c_caption = "Myth of the Ruby Sword";
const int MainWindow::c_fps = 60; // logic frames per second

MainWindow::MainWindow() :
    m_videoModeFlags(c_width, c_height, c_colorDepth),
    m_windowStyle(sf::Style::Close),
    m_window(NULL),
    m_gameplay(NULL)
{
    // set fullscreen flag
    setFullscreenFlags(Config::instance()->fullscreen());

    m_window = new sf::RenderWindow(m_videoModeFlags, c_caption, m_windowStyle);
    m_window->UseVerticalSync(true);

    // initialize gameplay
    if (m_window->IsOpened()) {
        m_gameplay = new Gameplay(this);
        if (! m_gameplay->isGood()) {
            std::cerr << "Gameplay did not initialize properly." << std::endl;
            std::exit(1);
        }
    }
}

MainWindow::~MainWindow()
{
    if (m_window)
        delete m_window;
    if (m_gameplay)
        delete m_gameplay;
}

int MainWindow::exec()
{
    sf::Clock clock;
    float goalTime = clock.GetElapsedTime();
    float interval = 1.0f / c_fps;

    while (m_window->IsOpened()) {
        // Process events
        sf::Event event;
        while (m_window->GetEvent(event)) {
            if (event.Type == sf::Event::Closed)
                m_window->Close();
        }

        // next logic frame in the gameplay
        m_gameplay->nextFrame();
        goalTime += interval;

        // draw. frame skip if we're too late!
        if (clock.GetElapsedTime() < goalTime) {
            m_gameplay->updateDisplay();
            m_window->Display();
        }
    }

    return (m_gameplay != NULL && m_gameplay->isGood()) ? EXIT_SUCCESS : EXIT_FAILURE;
}

void MainWindow::toggleFullscreen()
{
    setFullscreenFlags(! m_fullscreen);
    m_window->Create(m_videoModeFlags, c_caption, m_windowStyle);
}

void MainWindow::setFullscreenFlags(bool fullscreen)
{
    m_fullscreen = fullscreen;
    if( m_fullscreen )
        m_windowStyle |= sf::Style::Fullscreen;
    else
        m_windowStyle &= ~sf::Style::Fullscreen;
}


