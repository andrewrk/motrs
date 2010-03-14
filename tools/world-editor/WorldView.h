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

class EditorUniverse;

class WorldView : public QWidget
{
    Q_OBJECT
public: //variables
    enum MouseTool {
        mtNothing,
        mtArrow,
        mtEraser,
        mtPan,
        mtCenter,
        mtPencil,
        mtBrush,
        mtCreateMap,
        mtSetRealStartPoint,
        mtSetTestStartPoint,
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
    void guiSave();

    // prompt to save if necessary. returns false if cancelled
    bool guiEnsureSaved();

    // modifiers for the current selection. acts on objects and entities
    void selectAll();
    void deleteSelection();
    void copySelection();
    void paste();

    // true if we have unsaved changes
    inline bool tainted() const;
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
        msNormal,
        msStretchMapLeft,
        msStretchMapTop,
        msStretchMapRight,
        msStretchMapBottom,
        msMoveMap,
        msMoveSelectedItems,
        msCreateMap,
        msPan,
        msSelectionRectangle,
    };

    struct ArtItem {
        // representation
        QPixmap * pixmap;
        // location on disk
        QString artFile;
        // location in the game
        int x;
        int y;
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

        void moveByDelta(int deltaX, int deltaY) const
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

        // relative to parent map
        int x() const {
            if (isNull())
                return -1;
            if (type == sitEditorEntity) {
                return entity->centerX();
            } else if (type == sitMapObject){
                return object->tileX * Tile::size;
            }
            return -1.0;
        }

        // relative to parent map
        int y() const {
            if (isNull())
                return -1;
            if (type == sitEditorEntity) {
                return entity->centerY();
            } else if (type == sitMapObject){
                return object->tileY * Tile::size;
            }
            return -1.0;
        }

        EditorMap * parentMap()
        {
            if (isNull())
                return NULL;

            if (type == sitEditorEntity) {
                return entity->parent();
            } else if (type == sitMapObject){
                return object->parent;
            }

            return NULL;
        }

        void setParentMap(EditorMap * map)
        {
            if (isNull())
                return;

            if (type == sitEditorEntity) {
                map->addEntity(entity);
            } else if (type == sitMapObject){
                map->addObject(object);
            }
        }


        bool equals(const SelectableItem & item)
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

        // stores the location for dragging
        int mouseDownX;
        int mouseDownY;

    private:
        static int rectArea(QRect rect)
        {
            return rect.width() * rect.height();
        }

    };

    QScrollBar * m_hsb;
    QScrollBar * m_vsb;

    WorldEditor * m_window;
    EditorWorld * m_world;

    // where is the editor scrolled to, in absolute coordinates
    double m_zoom;
    int m_offsetX;
    int m_offsetY;

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
    int m_mouseDownHScroll;
    int m_mouseDownVScroll;

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

    EditorUniverse * m_universe;
    EditorUniverse * m_testUniverse;

    QPixmap * m_startPixmap;
    QPixmap * m_testStartPixmap;

    // do we have unsaved changes?
    bool m_tainted;

private: //methods
    // world coordinates to screen coordinates
    int screenX(int absoluteX);
    int screenY(int absoluteY);
    QRect screenRect(QRect absoluteRect);

    // screen coordinates to world coordinates
    int absoluteX(int screenX);
    int absoluteY(int screenY);
    QRect absoluteRect(QRect screenRect);

    // screen coordinates to selected map coordinates
    int mapX(int screenX, EditorMap * map);
    int mapY(int screenY, EditorMap * map);
    QRect mapRect(QRect screenRect, EditorMap * map);

    // map coordinates to screen coordinates
    QRect mapToScreenRect(QRect mapRect, EditorMap * map);
    int mapToScreenX(int mapX, EditorMap * map);
    int mapToScreenY(int mapY, EditorMap * map);

    int snapScreenX(int x);
    int snapScreenY(int y);
    int snapAbsoluteX(int x);
    int snapAbsoluteY(int y);

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
    void deselectItem(SelectableItem & item);

    // returns whether any items were selected
    bool selectItemsInRegion(QRect screenRegion);

    void moveSelectedItems(int deltaX, int deltaY);
    void saveSelectionMouseDownCoords();
    bool itemIsSelected(const SelectableItem & item);
    void deleteSelectableItem(const SelectableItem & item);

    void drawSelectedObjectAt(int x, int y);

    void setStartingPoint(QString universeFile, int screenX, int screenY);

    // load the locations of the real and test starting points so that
    // we can display them
    void updateUniverseCache();

    void drawStartBox(QPainter & p, EditorUniverse * universe, QPixmap * pixmap);

    // make sure the title bar displays the correct thing
    void refreshCaption();

    // mark the world as unsaved
    void taint();

    void adjustItemPositions(EditorMap * map, int tilesX, int tilesY);

private slots:
    void verticalScroll(int value);
    void horizontalScroll(int value);

};



inline bool WorldView::tainted() const
{
    return m_tainted;
}

#endif // WORLDVIEW_H
