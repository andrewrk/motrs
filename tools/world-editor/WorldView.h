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

    // modifiers for the current selection. acts on objects and entities
    void selectAll();
    void deleteSelection();
    void copySelection();
    void paste();
protected:
    void resizeEvent(QResizeEvent * e);
    void paintEvent(QPaintEvent * e);
    void mousePressEvent(QMouseEvent * e);
    void mouseReleaseEvent(QMouseEvent * e);
    void mouseMoveEvent(QMouseEvent * e);
    void wheelEvent(QWheelEvent * e);
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
        MoveSelectedItems,
    };

    struct ArtItem {
        // representation
        QPixmap * pixmap;
        // location on disk
        QString artFile;
        // location in the game
        double x;
        double y;
        int layer;
        EditorMap * map;
    };

    enum SelectableItemType {
        sitMapObject,
        sitEditorEntity,
    };

    class SelectableItem {
    public:
        // NULL constructor
        SelectableItem() :
            type(sitMapObject),
            object(NULL),
            entity(NULL)
        {
        }

        SelectableItem(EditorMap::MapObject * _object) :
            type(sitMapObject),
            object(_object),
            entity(NULL)
        {
        }

        SelectableItem(EditorEntity * _entity) :
            type(sitEditorEntity),
            object(NULL),
            entity(_entity)
        {
        }

        bool isNull() const
        {
            return (type == sitMapObject) ? (object == NULL) : (entity == NULL);
        }

        // return the area of the item
        int area() const
        {
            if (type == sitEditorEntity) {
                if (entity == NULL)
                    return 0;
                else
                    return rectArea(entity->geometry());
            } else if (type == sitMapObject) {
                if (object == NULL)
                    return 0;
                else
                    return rectArea(object->geometry());
            }
            return 0;
        }

        void saveMouseDownCoords()
        {
            if (isNull())
                return;
            if (type == sitEditorEntity) {
                mouseDownX = entity->centerX();
                mouseDownY = entity->centerY();
            } else if (type == sitMapObject) {
                mouseDownX = object->tileX * Tile::sizeInt;
                mouseDownY = object->tileY * Tile::sizeInt;
            }
        }

        void moveByDelta(double deltaX, double deltaY) const
        {
            if (isNull())
                return;
            if (type == sitEditorEntity) {
                entity->setCenter(mouseDownX+deltaX, mouseDownY+deltaY);
            } else if (type == sitMapObject) {
                object->tileX = (int) round((mouseDownX+deltaX) / Tile::size);
                object->tileY = (int) round((mouseDownY+deltaY) / Tile::size);
            }
        }

        bool equals(SelectableItem & item)
        {
            if (item.type == type) {
                if (item.type == sitEditorEntity)
                    return item.entity == entity;
                else if (item.type == sitMapObject)
                    return item.object == object;
            }
            return false;
        }

        SelectableItemType type;
        EditorMap::MapObject * object;
        EditorEntity * entity;


    private:
        static int rectArea(QRectF rect)
        {
            return rect.width() * rect.height();
        }

        // stores the location for dragging
        double mouseDownX;
        double mouseDownY;
    };

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

    QList<SelectableItem *> m_selection;

private: //methods
    // transfer between absolute coordinates and editor coordinates
    int screenX(double absoluteX);
    int screenY(double absoluteY);
    QRect screenRect(QRectF absoluteRect);
    double absoluteX(int screenX);
    double absoluteY(int screenY);
    QRectF absoluteRect(QRect screenRect);

    int snapScreenX(int x);
    int snapScreenY(int y);
    double snapAbsoluteX(double x);
    double snapAbsoluteY(double y);

    void drawGrid(QPainter &p);
    void updateViewCache();
    void updateScrollBars();

    // determining if cursor is in range of resizing maps
    bool overMapLeft(int x, int y);
    bool overMapRight(int x, int y);
    bool overMapTop(int x, int y);
    bool overMapBottom(int x, int y);
    bool overSelectedMap(int x, int y);

    // if we're over it, return it, otherwise return null
    EditorMap * mapAt(int x, int y);
    SelectableItem selectableItemAt(int x, int y);

    void refreshLayersList();
    void setControlEnableStates();
    void determineCursor();

    void drawObject(QPainter &p, EditorMap::MapObject * object, int layerIndex, EditorMap * owner);

    void selectMap(EditorMap * map);
    void selectOnly(SelectableItem item);
    void selectAlso(SelectableItem item);
    void selectNone();

    void moveSelectedItems(double deltaX, double deltaY);
    void saveSelectionMouseDownCoords();
    bool itemIsSelected(SelectableItem item);


private slots:
    void verticalScroll(int value);
    void horizontalScroll(int value);

};

#endif // WORLDVIEW_H
