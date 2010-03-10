#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <SFML/Graphics.hpp>

class Gameplay;

// Main window is responsible for setting up the graphics framework,
// Gameplay, and handling window events.
// MainWindow has the main loop, which calls the necessary Gameplay methods
// to make Gameplay work
class MainWindow
{
public:
    MainWindow();
    ~MainWindow();

    // main loop and window processing
    int exec();

    // switch between full screen and windowed mode
    void toggleFullscreen();

    // return the rendering window
    inline sf::RenderWindow * renderWindow();

    // number of logic frames per second
    inline int fps();

    // close window and end gameplay
    void close();
private: //variables
    static const int c_fps;
    static const int c_width;
    static const int c_height;
    static const char * c_caption;
    static const int c_colorDepth;

    bool m_fullscreen;
    sf::VideoMode m_videoModeFlags;
    int m_windowStyle;
    sf::RenderWindow * m_window;
    Gameplay * m_gameplay;
private: //methods
    void setFullscreenFlags(bool fullscreen);
};

inline sf::RenderWindow * MainWindow::renderWindow()
{
    return m_window;
}

inline int MainWindow::fps()
{
    return c_fps;
}

#endif // MAINWINDOW_H
