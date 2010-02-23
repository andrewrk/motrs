#ifndef WORLDVIEW_H
#define WORLDVIEW_H

#include <QWidget>
#include <QScrollBar>
#include <QPaintEvent>
#include <QResizeEvent>
#include <QTimer>

#include "EditorWorld.h"
#include "EditorMap.h"
#include "EditorEntity.h"
#include "WorldEditor.h"

class WorldView : public QWidget
{
    Q_OBJECT
public: //variables
    enum MouseTool {
        Nothing,
        Arrow,
        Eraser,
        Pan,
        Center,
        Pencil,
        Brush,
    };
public: //methods
    WorldView(WorldEditor * window, QWidget * parent = NULL);
    ~WorldView();

    void setWorld(EditorWorld * world);
    void setSelectedLayer(int index);

    void addLayer();
    void swapLayers(int i, int j);
    void deleteLayer(int index);

    MouseTool toolLeftClick() { return m_toolLeftClick; }
    void setToolLeftClick(MouseTool tool);

    MouseTool toolMiddleClick() { return m_toolMiddleClick; }
    void setToolMiddleClick(MouseTool tool);

    MouseTool toolRightClick() { return m_toolRightClick; }
    void setToolRightClick(MouseTool tool);

    void refreshGui();
    void refreshObjectsList();

    // save the opened world to the data folder
    void saveTheWorld();
protected:
    void resizeEvent(QResizeEvent * e);
    void paintEvent(QPaintEvent * e);
    void mousePressEvent(QMouseEvent * e);
    void mouseReleaseEvent(QMouseEvent * e);
    void mouseMoveEvent(QMouseEvent * e);
    void keyPressEvent(QKeyEvent * e);
    void keyReleaseEvent(QKeyEvent * e);

    void dropEvent(QDropEvent * e);
    void dragEnterEvent(QDragEnterEvent * e);
    void dragMoveEvent(QDragMoveEvent * e);
    void dragLeaveEvent(QDragLeaveEvent * e);

private: //variables
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

    QScrollBar * m_hsb;
    QScrollBar * m_vsb;

    WorldEditor * m_window;
    EditorWorld * m_world;

    // where is the editor scrolled to, in absolute coordinates
    double m_zoom;
    double m_offsetX;
    double m_offsetY;

    // saved list of maps that are visible for fast rendering
    QList<EditorMap *> m_mapCache;
    // highest number of layers of all visible maps
    int m_maxLayer;

    EditorMap * m_selectedMap;
    int m_selectedLayer;

    int m_mouseDownX;
    int m_mouseDownY;
    MouseTool m_mouseDownTool;
    int m_mouseState;

    // location of the mouse
    int m_mouseX;
    int m_mouseY;
    Qt::KeyboardModifiers m_keyboardModifiers;

    // art that has not been converted into objects or entities yet.
    ArtItem m_tempArt;

    // what tools to use when we click with the mouse
    MouseTool m_toolLeftClick;
    MouseTool m_toolMiddleClick;
    MouseTool m_toolRightClick;

    QTimer m_animationTimer;

    EditorMap::MapObject * m_dragObject;

private: //methods
    // transfer between absolute coordinates and editor coordinates
    int screenX(double absoluteX);
    int screenY(double absoluteY);
    double absoluteX(int screenX);
    double absoluteY(int screenY);

    int snapScreenX(int x);
    int snapScreenY(int y);
    double snapAbsoluteX(double x);
    double snapAbsoluteY(double y);

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

    void drawObject(QPainter &p, EditorMap::MapObject * object, int layerIndex, EditorMap * owner);

private slots:
    void verticalScroll(int value);
    void horizontalScroll(int value);

};

#endif // WORLDVIEW_H
