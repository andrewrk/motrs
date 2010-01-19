#ifndef WORLDVIEW_H
#define WORLDVIEW_H

#include <QWidget>
#include <QScrollBar>
#include <QPaintEvent>
#include <QResizeEvent>

#include "editorworld.h"

class MainWindow;

class WorldView : public QWidget
{
    Q_OBJECT
public:
    WorldView(MainWindow * window, QWidget * parent = NULL);
    ~WorldView();

    void setWorld(EditorWorld * world);
protected:
    void resizeEvent(QResizeEvent * e);
    void paintEvent(QPaintEvent * e);
    void mousePressEvent(QMouseEvent * e);
private:
    QScrollBar * m_hsb;
    QScrollBar * m_vsb;

    MainWindow * m_window;
    EditorWorld * m_world;
    double m_zoom;

    // where is the editor scrolled to, in absolute coordinates
    double m_offsetX;
    double m_offsetY;

    // transfer between absolute coordinates and editor coordinates
    double screenX(double absoluteX);
    double screenY(double absoluteY);
    double absoluteX(double screenX);
    double absoluteY(double screenY);


    friend class MainWindow;
};

#endif // WORLDVIEW_H
