#ifndef EDITORGRAPHIC_H
#define EDITORGRAPHIC_H

#include "Graphic.h"
#include "EditorGraphicTimer.h"

#include <QPainter>
#include <QList>
#include <QPixmap>

// provides a unified API to the editor regarding animations and graphics
class EditorGraphic
{
    friend class EditorGraphicTimer;
public:
    // initialize the animation timer
    static void initialize();

    // supply a full path and returns an EditorGraphic
    static EditorGraphic * load(QString file);

    void draw(QPainter & p, int x, int y, int width, int height);
    void draw(QPainter & p, int x, int y);

    // get the size in pixels
    inline int width();
    inline int height();

    // return a pixmap
    QPixmap * toPixmap(int frame = -1);

private: //variables
    static const int c_gameFps;
    static long long int s_frameCount; // total frames passed by in the duration of the game
    static EditorGraphicTimer * s_timer; // calls the updateFrameCount method on a timer

    QList<QPixmap *> m_frames;

    int m_width;
    int m_height;

    int m_frameCount;
    int m_fps;
    int m_offset;

private: //methods
    EditorGraphic();

    static void updateFrameCount();
    int currentFrame();

};

inline int EditorGraphic::width()
{
    return m_width;
}

inline int EditorGraphic::height()
{
    return m_height;
}


#endif // EDITORGRAPHIC_H
