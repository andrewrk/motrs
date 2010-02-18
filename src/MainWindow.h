#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ArgumentParser.h"
#include "SDL.h"

class Gameplay;

// Main window is responsible for setting up SDL and Gameplay and
// handling window events
class MainWindow
{
public:
    MainWindow(int argc, char * argv[]);
    ~MainWindow();

    // main loop and window processing
    int exec();

    void toggleFullscreen();
private:
    static const int c_fps;
    static const int c_width;
    static const int c_height;
    static const char * c_caption;
    static const int c_colorDepth;

    ArgumentParser m_arguments;
    bool m_fullscreen;
    SDL_Surface * m_screen;
    Gameplay * m_gameplay;
};

#endif // MAINWINDOW_H
