#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "SDL.h"

class Gameplay;

// Main window is responsible for setting up SDL and Gameplay and
// handling window events
class MainWindow
{
public:
    MainWindow();
    ~MainWindow();

    // main loop and window processing
    int exec();

    void toggleFullscreen();
private: //variables
    static const int c_fps;
    static const int c_width;
    static const int c_height;
    static const char * c_caption;
    static const int c_colorDepth;

    bool m_fullscreen;
    SDL_Surface * m_screen;
    Gameplay * m_gameplay;
    Uint32 m_videoModeFlags;
private: //methods
    void setFullscreenFlags(bool fullscreen);
};

#endif // MAINWINDOW_H
