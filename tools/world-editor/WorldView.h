#ifndef WORLDVIEW_H
#define WORLDVIEW_H

#include <QWidget>
#include <QScrollBar>
#include <QPaintEvent>
#include <QResizeEvent>

#include "EditorWorld.h"
#include "EditorMap.h"
#include "EditorEntity.h"
#include "WorldEditor.h"

class WorldView : public QWidget
{
    Q_OBJECT
    friend class WorldEditor;
public:
    WorldView(WorldEditor * window, QWidget * parent = NULL);
    ~WorldView();

    void setWorld(EditorWorld * world);
    void setSelectedLayer(int index);

    void addLayer();
    void swapLayers(int i, int j);
    void deleteLayer(int index);

    inline static QPainter * painter() { return s_painter; }
protected:
    void resizeEvent(QResizeEvent * e);
    void paintEvent(QPaintEvent * e);
    void mousePressEvent(QMouseEvent * e);
    void mouseReleaseEvent(QMouseEvent * e);
    void mouseMoveEvent(QMouseEvent * e);
    void keyPressEvent(QKeyEvent * e);
    void keyReleaseEvent(QKeyEvent * e);
private:
    enum MouseState {
        Normal,
        SetStartPoint,
        StretchMapLeft,
        StretchMapTop,
        StretchMapRight,
        StretchMapBottom,
        MoveMap,
    };

    typedef struct {
        // representation
        QPixmap * pixmap;
        // location on disk
        QString artFile;
        // location in the game
        double x;
        double y;
        int layer;
        EditorMap * map;
    } ArtItem;

    // how far away from border lines can you be to be able to use stretch tools
    static const int s_lineSelectRadius;

    QScrollBar * m_hsb;
    QScrollBar * m_vsb;

    WorldEditor * m_window;
    EditorWorld * m_world;

    // where is the editor scrolled to, in absolute coordinates
    double m_zoom;
    double m_offsetX;
    double m_offsetY;

    // saved list of maps that are visible for fast rendering
    QVector<EditorMap *> m_mapCache;
    QVector<EditorEntity *> m_entityCache;
    // highest number of layers of all visible maps
    int m_maxLayer;

    EditorMap * m_selectedMap;
    int m_selectedLayer;

    int m_mouseDownX;
    int m_mouseDownY;
    WorldEditor::MouseTool m_mouseDownTool;
    int m_mouseState;

    // location of the mouse
    int m_mouseX;
    int m_mouseY;
    Qt::KeyboardModifiers m_keyboardModifiers;

    // holds QPainter object for drawing
    static QPainter * s_painter;

    // art that has not been converted into objects or entities yet.
    ArtItem m_tempArt;


    // transfer between absolute coordinates and editor coordinates
    double screenX(double absoluteX);
    double screenY(double absoluteY);
    double absoluteX(double screenX);
    double absoluteY(double screenY);

    void drawGrid(QPainter &p);

    void updateViewCache();

    void selectMap(EditorMap * map);
    EditorMap * mapAt(int x, int y);

    // determining if cursor is in range of resizing maps
    bool overMapLeft(int x, int y);
    bool overMapRight(int x, int y);
    bool overMapTop(int x, int y);
    bool overMapBottom(int x, int y);
    bool overSelectedMap(int x, int y);
    EditorMap * overAnyMap(int x, int y);

    void determineCursor();

    void refreshLayersList();
    void setControlEnableStates();


private slots:
    void verticalScroll(int value);
    void horizontalScroll(int value);

};

#endif // WORLDVIEW_H
