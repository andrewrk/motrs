#ifndef WORLDVIEW_H
#define WORLDVIEW_H

#include <QWidget>
#include <QScrollBar>
#include <QPaintEvent>
#include <QResizeEvent>

#include "EditorWorld.h"
#include "EditorMap.h"

class MainWindow;

class WorldView : public QWidget
{
    Q_OBJECT
public:
    enum GridRenderType {
        None,
        Fast,
        Pretty,
    };

    WorldView(MainWindow * window, QWidget * parent = NULL);
    ~WorldView();

    void setWorld(EditorWorld * world);
    void setSelectedLayer(int index);
protected:
    void resizeEvent(QResizeEvent * e);
    void paintEvent(QPaintEvent * e);
    void mousePressEvent(QMouseEvent * e);
    void mouseMoveEvent(QMouseEvent * e);
private:
    // how far away from border lines can you be to be able to use stretch tools
    static const int s_lineSelectRadius;

    QScrollBar * m_hsb;
    QScrollBar * m_vsb;

    MainWindow * m_window;
    EditorWorld * m_world;
    double m_zoom;

    // where is the editor scrolled to, in absolute coordinates
    double m_offsetX;
    double m_offsetY;

    GridRenderType m_grid;

    // saved list of maps that are visible for fast rendering
    QVector<EditorMap *> m_mapCache;
    // highest number of layers of all visible maps
    int m_maxLayer;

    EditorMap * m_selectedMap;
    int m_selectedLayer;

    int m_mouseDownX;
    int m_mouseDownY;

    // transfer between absolute coordinates and editor coordinates
    double screenX(double absoluteX);
    double screenY(double absoluteY);
    double absoluteX(double screenX);
    double absoluteY(double screenY);

    void readSettings();

    void drawGrid(QPainter &p);

    void updateViewCache();

    void selectMap(EditorMap * map);
    EditorMap * mapAt(int x, int y);

    // determining if cursor is in range of resizing maps
    bool overMapLeft(int x, int y);
    bool overMapRight(int x, int y);
    bool overMapTop(int x, int y);
    bool overMapBottom(int x, int y);


private slots:
    void verticalScroll(int value);
    void horizontalScroll(int value);

    friend class MainWindow;
};

#endif // WORLDVIEW_H
