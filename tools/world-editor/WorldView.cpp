#include "WorldView.h"

#include "WorldEditor.h"
#include "EditorResourceManager.h"
#include "EditorSettings.h"
#include "EditorGlobals.h"
#include "EditorUniverse.h"

#include <QApplication>
#include <QPainter>
#include <QDebug>
#include <QListWidget>
#include <QList>
#include <QUrl>
#include <QFileInfo>
#include <QMessageBox>
#include <QDir>
#include <QStandardItemModel>
#include <QRect>

#include <cmath>

#include "moc_WorldView.cxx"

WorldView::WorldView(WorldEditor * window, QWidget * parent) :
    QWidget(parent),
    m_hsb(new QScrollBar(Qt::Horizontal, this)),
    m_vsb(new QScrollBar(Qt::Vertical, this)),
    m_window(window),
    m_world(NULL),
    m_zoom(1.0),
    m_offsetX(0),
    m_offsetY(0),
    m_mapCache(),
    m_selectedMap(NULL),
    m_mouseDownTool(mtNothing),
    m_mouseState(msNormal),
    m_dragObject(NULL),
    m_universe(NULL),
    m_testUniverse(NULL),
    m_tainted(false)
{
    QDir localData(EditorResourceManager::localDataDir());
    m_startPixmap = new QPixmap(localData.absoluteFilePath("StartBox.png"));
    m_testStartPixmap = new QPixmap(localData.absoluteFilePath("TestStartBox.png"));

    m_hsb->show();
    m_vsb->show();

    connect(m_vsb, SIGNAL(valueChanged(int)), this, SLOT(verticalScroll(int)));
    connect(m_hsb, SIGNAL(valueChanged(int)), this, SLOT(horizontalScroll(int)));

    this->setMouseTracking(true);
    this->setAcceptDrops(true);

    // refresh the display to animate
    connect(&m_animationTimer, SIGNAL(timeout()), this, SLOT(update()));
    m_animationTimer.start(100);

    updateUniverseCache();
    updateViewCache();
}

WorldView::~WorldView()
{
    delete m_startPixmap;
    delete m_testStartPixmap;
    delete m_world;
    delete m_universe;
    delete m_testUniverse;
}

void WorldView::refreshGui()
{
    updateUniverseCache();
    refreshLayersList();
    refreshObjectsList();
    setControlEnableStates();
    refreshCaption();
}

void WorldView::resizeEvent(QResizeEvent * e)
{
    // move the scroll bars into position
    m_hsb->setGeometry(0, this->height()-m_hsb->height(), this->width()-m_vsb->width(), m_hsb->height());
    m_vsb->setGeometry(this->width()-m_vsb->width(), 0, m_vsb->width(), this->height()-m_hsb->height());

    updateViewCache();
    update();
}

void WorldView::updateViewCache()
{
    // when the scroll or zoom changes, we need to recalculate which maps
    // need to be drawn.
    m_mapCache.clear();
    if( m_world ) {
        // select the maps that are in range
        const QList<EditorMap *> * maps = m_world->maps();
        int viewLeft = absoluteX(0);
        int viewTop = absoluteY(0);
        int viewRight = viewLeft + this->width() * m_zoom;
        int viewBottom = viewTop + this->height() * m_zoom;
        m_maxLayer = 0;
        for(int i=0; i < maps->size(); ++i) {
            // determine if the map is in range
            EditorMap * map = maps->at(i);

            if (!(map->left() > viewRight || map->top() > viewBottom ||
                  map->left() + map->width() < viewLeft || map->top() + map->height() < viewTop))
            {
                if (map->layerCount() > m_maxLayer)
                    m_maxLayer = map->layerCount();

                m_mapCache.append(map);
            }
        }
    }

    this->update();
}

void WorldView::paintEvent(QPaintEvent * e)
{
    QPainter p(this);
    p.setBackground(Qt::white);
    p.eraseRect(0, 0, this->width(), this->height());

    int deltaX = m_mouseX - m_mouseDownX;
    int deltaY = m_mouseY - m_mouseDownY;

    if( m_world ) {
        drawGrid(p);

        for(int layerIndex=0; layerIndex<m_maxLayer; ++layerIndex) {
            // draw map at this layer
            for(int mapIndex=0; mapIndex<m_mapCache.size(); ++mapIndex) {
                EditorMap * map = m_mapCache.at(mapIndex);

                // if the layer is greater than the selected map's number of layers, skip
                if (layerIndex >= m_window->layersList()->count())
                    continue;

                // if the map is selected and this layer is unchecked, don't draw
                if (map == m_selectedMap && m_window->layersList()->item(layerIndex)->checkState() == Qt::Unchecked)
                    continue;

                if (layerIndex < map->layerCount()) {
                    // draw only this layer from the map
                    const EditorMap::MapLayer * layer = map->layer(layerIndex);

                    // Objects. need to check every layer before this one and
                    // this one because objects can have multiple layers.
                    for (int objectIndex=0; objectIndex<layer->objects.size(); ++objectIndex) {
                        // object is any EditorObject whose layers overlap layerIndex. How convenient!
                        EditorMap::MapObject * object = layer->objects.at(objectIndex);
                        drawObject(p, object, layerIndex, map);
                    }

                    // Object being dragged from the objects list
                    if (m_dragObject != NULL) {
                        if (layerIndex >= m_dragObject->layer &&
                            layerIndex < m_dragObject->layer + m_dragObject->object->layerCount())
                        {
                            drawObject(p, m_dragObject, layerIndex, m_selectedMap);
                        }
                    }

                    // Entities. Only have one graphic and one layer. nice and simple.
                    for (int entityIndex=0; entityIndex<layer->entities.size(); ++entityIndex) {
                        EditorEntity * entity = layer->entities.at(entityIndex);
                        p.drawPixmap(
                            screenX(map->left() + entity->centerX() - entity->centerOffsetX()),
                            screenY(map->top() + entity->centerY() - entity->centerOffsetY()),
                            entity->width() * m_zoom, entity->height() * m_zoom,
                            *entity->graphic()->toPixmap());
                    }
                }
            }
        }

        // selection rectangles
        p.setPen(QPen(Qt::blue, 2));
        p.setBrush(Qt::NoBrush);
        for (int i=0; i<m_selection.size(); ++i) {
            const SelectableItem * item = m_selection.at(i);
            QRect bound;
            if (item->type == sitEditorEntity) {
                bound = item->entity->geometry();
            } else if (item->type == sitMapObject) {
                bound = item->object->geometry();
            }
            QRect screenBound = mapToScreenRect(bound, m_selectedMap);
            p.drawRect(screenBound);
        }

        // draw a bold line around map borders
        p.setBrush(Qt::NoBrush);
        p.setPen(QPen(Qt::black, 3));
        for (int i = 0; i < m_mapCache.size(); i++)
            p.drawRect(screenRect(m_mapCache[i]->geometry()));

        // draw start point and test start point
        drawStartBox(p, m_universe, m_startPixmap);
        drawStartBox(p, m_testUniverse, m_testStartPixmap);

        // draw selected map outline in blue
        if (m_selectedMap != NULL) {
            p.setPen(QPen(Qt::blue, 3));

            QRect outline = screenRect(m_selectedMap->geometry());

            // if we're dragging boundaries, move the blue line
            if (m_mouseState == msStretchMapLeft)
                outline.setLeft(snapScreenX(outline.left() + deltaX));
            else if (m_mouseState == msStretchMapBottom)
                outline.setBottom(snapScreenY(outline.bottom() + deltaY));
            else if (m_mouseState == msStretchMapRight)
                outline.setRight(snapScreenX(outline.right() + deltaX));
            else if (m_mouseState == msStretchMapTop)
                outline.setTop(snapScreenY(outline.top() + deltaY));
            else if (m_mouseState == msMoveMap)
                outline.moveTo(snapScreenX(outline.left() + deltaX), snapScreenY(outline.top() + deltaY));

            p.drawRect(outline);
        }

        if (m_mouseState == msCreateMap) {
            // draw a line for creating a new map
            p.setPen(QPen(Qt::red, 3));
            QRect outline;

            outline.setLeft(snapScreenX(m_mouseDownX));
            outline.setTop(snapScreenY(m_mouseDownY));
            outline.setRight(snapScreenX(m_mouseX));
            outline.setBottom(snapScreenY(m_mouseY));

            p.drawRect(outline);
        } else if (m_mouseState == msSelectionRectangle) {
            // selection rectangle
            p.setPen(QPen(Qt::red, 1));
            QRect outline(m_mouseDownX, m_mouseDownY, deltaX, deltaY);
            p.drawRect(outline);
        }



    } else {
        p.drawText(0, 0, this->width(), this->height(), Qt::AlignCenter,
            tr("Double click a world to edit"));
    }
}

void WorldView::drawStartBox(QPainter &p, EditorUniverse *universe, QPixmap *pixmap)
{
    if (universe == NULL)
        return;
    if (m_world == NULL)
        return;
    if (universe->startWorldName().compare(m_world->name()) != 0)
        return;
    int arbitraryWidth = 40;
    int arbitraryHeight = 40;
    int drawX = screenX(universe->startX()) - arbitraryWidth / 2;
    int drawY = screenY(universe->startY()) - arbitraryHeight / 2;
    p.drawPixmap(drawX, drawY, arbitraryWidth, arbitraryHeight, *pixmap);
}

void WorldView::drawObject(QPainter &p, EditorMap::MapObject * object, int layerIndex, EditorMap * map)
{
    // paint all the graphics which are at the layer we want
    QList<EditorObject::ObjectGraphic *> * graphics = object->object->graphics()->at(layerIndex);
    for (int i=0; i<graphics->size(); ++i) {
        EditorObject::ObjectGraphic * graphic = graphics->at(i);
        p.drawPixmap(
            screenX(map->left() + object->tileX * Tile::size + graphic->x),
            screenY(map->top() + object->tileY * Tile::size + graphic->y),
            graphic->width * m_zoom, graphic->height * m_zoom,
            *graphic->graphic->toPixmap());
    }
}

void WorldView::drawGrid(QPainter &p)
{
    EditorSettings::GridRenderType gridType = EditorSettings::gridRenderType();
    if( gridType != EditorSettings::None ) {
        if( gridType == EditorSettings::Pretty )
            p.setPen(QColor(128, 128, 128, 64));
        else if( gridType == EditorSettings::Solid )
            p.setPen(Qt::black);
        else
            p.setPen(QColor(128, 128, 128));

        int gameLeft = absoluteX(0);
        int gameTop = absoluteY(0);
        int gameRight = absoluteX(this->width());
        int gameBottom = absoluteY(this->height());
        int gridX = gameLeft - std::fmod(gameLeft, Tile::size);
        int gridY = gameTop - std::fmod(gameTop, Tile::size);

        if( gridType == EditorSettings::Pretty || gridType == EditorSettings::Solid ) {
            while(gridX < gameRight) {
                int drawX = screenX(gridX);
                p.drawLine((int)drawX, 0, (int)drawX, this->height());
                gridX += Tile::size;
            }

            while(gridY < gameBottom) {
                int drawY = screenY(gridY);
                p.drawLine(0, (int)drawY, this->width(), (int)drawY);
                gridY += Tile::size;
            }
        } else if( gridType == EditorSettings::Fast ) {
            for(int y = gridY; y < gameBottom; y+=Tile::size) {
                for(int x = gridX; x < gameRight; x+=Tile::size)
                    p.drawPoint((int)screenX(x), (int)screenY(y));
            }
        }
    }
}


QRect WorldView::screenRect(QRect absoluteRect)
{
    QRect out;
    out.setX(screenX(absoluteRect.x()));
    out.setY(screenY(absoluteRect.y()));
    out.setWidth(absoluteRect.width() * m_zoom);
    out.setHeight(absoluteRect.height() * m_zoom);
    return out;
}

QRect WorldView::absoluteRect(QRect screenRect)
{
    QRect out;
    out.setX(absoluteX(screenRect.x()));
    out.setY(absoluteY(screenRect.y()));
    out.setWidth(screenRect.width() / m_zoom);
    out.setHeight(screenRect.height() / m_zoom);
    return out;
}

int WorldView::screenX(int absoluteX)
{
    return (int)(absoluteX - m_offsetX) * m_zoom;
}

int WorldView::screenY(int absoluteY)
{
    return (int)(absoluteY - m_offsetY) * m_zoom;
}

QRect WorldView::mapToScreenRect(QRect mapRect, EditorMap * map)
{
    QRect out;
    out.setLeft(screenX(mapRect.left() + map->left()));
    out.setTop(screenY(mapRect.top() + map->top()));
    out.setWidth(mapRect.width() * m_zoom);
    out.setHeight(mapRect.height() * m_zoom);
    return out;
}

int WorldView::mapToScreenX(int mapX, EditorMap * map)
{
    return screenX(mapX + map->left());
}

int WorldView::mapToScreenY(int mapY, EditorMap * map)
{
    return screenY(mapY + map->top());
}

int WorldView::mapX(int screenX, EditorMap * map)
{
    return (screenX / m_zoom) + m_offsetX - map->left();
}

int WorldView::mapY(int screenY, EditorMap * map)
{
    return (screenY / m_zoom) + m_offsetY - map->top();
}


QRect WorldView::mapRect(QRect screenRect, EditorMap *map)
{
    QRect out;
    out.setLeft(mapX(screenRect.left(), map));
    out.setTop(mapY(screenRect.top(), map));
    out.setWidth(screenRect.width() / m_zoom);
    out.setHeight(screenRect.height() / m_zoom);
    return out;
}

int WorldView::absoluteX(int screenX)
{
    return (screenX / m_zoom) + m_offsetX;
}

int WorldView::absoluteY(int screenY)
{
    return (screenY / m_zoom) + m_offsetY;
}

int WorldView::snapScreenX(int x)
{
    // snap to tile boundary
    return screenX(snapAbsoluteX(absoluteX(x)));
}

int WorldView::snapScreenY(int y)
{
    return screenY(snapAbsoluteY(absoluteY(y)));
}

int WorldView::snapAbsoluteX(int x)
{
    return round(x / Tile::size) * Tile::size;
}

int WorldView::snapAbsoluteY(int y)
{
    return round(y / Tile::size) * Tile::size;
}

bool WorldView::overMapLeft(int x, int y)
{
    if( ! m_selectedMap )
        return false;

    int absX = absoluteX(x);
    int absY = absoluteY(y);

    return  absX > m_selectedMap->left() - g_lineSelectRadius &&
            absX < m_selectedMap->left() + g_lineSelectRadius &&
            absY > m_selectedMap->top() && absY < m_selectedMap->top() + m_selectedMap->height();
}

bool WorldView::overMapTop(int x, int y)
{
    if( ! m_selectedMap )
        return false;

    int absX = absoluteX(x);
    int absY = absoluteY(y);

    return  absX > m_selectedMap->left() && absX < m_selectedMap->left() + m_selectedMap->width() &&
            absY > m_selectedMap->top() - g_lineSelectRadius &&
            absY < m_selectedMap->top() + g_lineSelectRadius;
}
bool WorldView::overMapRight(int x, int y)
{
    if( ! m_selectedMap )
        return false;

    int absX = absoluteX(x);
    int absY = absoluteY(y);

    return  absX > m_selectedMap->left() + m_selectedMap->width() - g_lineSelectRadius &&
            absX < m_selectedMap->left() + m_selectedMap->width() + g_lineSelectRadius &&
            absY > m_selectedMap->top() && absY < m_selectedMap->top() + m_selectedMap->height();

}
bool WorldView::overMapBottom(int x, int y)
{
    if( ! m_selectedMap )
        return false;

    int absX = absoluteX(x);
    int absY = absoluteY(y);

    return  absX > m_selectedMap->left() && absX < m_selectedMap->left() + m_selectedMap->width() &&
            absY > m_selectedMap->top() + m_selectedMap->height() - g_lineSelectRadius &&
            absY < m_selectedMap->top() + m_selectedMap->height() + g_lineSelectRadius;
}

bool WorldView::overSelectedMap(int x, int y)
{
    if( ! m_selectedMap )
        return false;

    int absX = absoluteX(x);
    int absY = absoluteY(y);

    return  absX > m_selectedMap->left() && absX < m_selectedMap->left() + m_selectedMap->width() &&
            absY > m_selectedMap->top() && absY < m_selectedMap->top() + m_selectedMap->height();
}

void WorldView::keyPressEvent(QKeyEvent * e)
{
    m_keyboardModifiers = e->modifiers();
    determineCursor();

    e->ignore();
}

void WorldView::keyReleaseEvent(QKeyEvent * e)
{
    m_keyboardModifiers = e->modifiers();
    determineCursor();

    e->ignore();
}

void WorldView::determineCursor()
{
    switch(m_mouseState) {
    case msNormal: {
            switch(m_mouseDownTool) {
            case mtNothing:
                {
                    // change mouse cursor to sizers if over map boundaries
                    // if the user could use the arrow tool
                    if( m_toolLeftClick == mtArrow ||
                        m_toolMiddleClick == mtArrow ||
                        m_toolRightClick == mtArrow )
                    {
                        if( overMapLeft(m_mouseX, m_mouseY) || overMapRight(m_mouseX, m_mouseY))
                            this->setCursor(Qt::SizeHorCursor);
                        else if( overMapTop(m_mouseX, m_mouseY) || overMapBottom(m_mouseX, m_mouseY))
                            this->setCursor(Qt::SizeVerCursor);
                        else if( overSelectedMap(m_mouseX, m_mouseY) && m_keyboardModifiers & Qt::AltModifier)
                            this->setCursor(Qt::SizeAllCursor);
                        else
                            this->setCursor(Qt::ArrowCursor);
                    } else {
                        this->setCursor(Qt::ArrowCursor);
                    }
                    break;
                }
            case mtArrow: break;
            case mtEraser: break;
            case mtPan:
                this->setCursor(Qt::SizeAllCursor);
                break;
            case mtCenter: break;
            case mtPencil: break;
            case mtBrush: break;
            case mtCreateMap: break;
            case mtSetRealStartPoint: break;
            case mtSetTestStartPoint: break;
            }
        } break;
    case msStretchMapLeft: break;
    case msStretchMapTop: break;
    case msStretchMapRight: break;
    case msStretchMapBottom: break;
    case msMoveMap: break;
    case msPan:
        this->setCursor(Qt::SizeAllCursor);
    }
}

void WorldView::mouseMoveEvent(QMouseEvent * e)
{
    m_mouseX = e->x();
    m_mouseY = e->y();
    m_keyboardModifiers = e->modifiers();


    int screenDeltaX = e->x() - m_mouseDownX;
    int screenDeltaY = e->y() - m_mouseDownY;
    int deltaX = screenDeltaX * m_zoom;
    int deltaY = screenDeltaY * m_zoom;

    determineCursor();

    if (m_world == NULL)
        return;

    switch(m_mouseState) {
    case msNormal:
        {
            switch(m_mouseDownTool) {
            case mtNothing: break;
            case mtArrow: break;
            case mtEraser:
                {
                    // select the map under the cursor
                    EditorMap * newMap = mapAt(e->x(),e->y());

                    if (newMap != m_selectedMap)
                        selectMap(newMap);

                    if (m_selectedMap == NULL)
                        return;

                    // delete all objects and entities at this location
                    SelectableItem item = selectableItemAt(e->x(), e->y());
                    while (! item.isNull()) {
                        // delete this item
                        if (itemIsSelected(item))
                            deselectItem(item);
                        deleteSelectableItem(item);
                        item = selectableItemAt(e->x(), e->y());
                    }
                    taint();
                    this->update();
                    break;
                }
            case mtPan: break;
            case mtCenter: break;
            case mtPencil: break;
            case mtBrush:
                drawSelectedObjectAt(e->x(), e->y());
                break;
            case mtCreateMap: break;
            case mtSetRealStartPoint: break;
            case mtSetTestStartPoint: break;
            }
            break;
        }
        // update the screen when we're doing stuff
    case msStretchMapLeft:
    case msStretchMapTop:
    case msStretchMapRight:
    case msStretchMapBottom:
    case msMoveMap:
    case msCreateMap:
    case msSelectionRectangle:
        this->update();
        break;
    case msMoveSelectedItems:
        moveSelectedItems(deltaX, deltaY);
        taint();
        this->update();
        break;
    case msPan:
        m_hsb->setValue(m_mouseDownHScroll - deltaX);
        m_vsb->setValue(m_mouseDownVScroll - deltaY);
        updateViewCache();
        break;
    }
}

void WorldView::moveSelectedItems(int deltaX, int deltaY)
{
    // if a destination is not in a map, refuse to move it
    for (int i=0; i<m_selection.size(); ++i) {
        SelectableItem * item = m_selection.at(i);

        int destX = mapToScreenX(item->mouseDownX + deltaX, item->parentMap());
        int destY = mapToScreenY(item->mouseDownY + deltaY, item->parentMap());

        EditorMap * destMap = mapAt(destX, destY);

        // TODO: allow changing parents
        if (destMap != item->parentMap())
            continue;

        item->moveByDelta(deltaX, deltaY);
    }
}

void WorldView::adjustItemPositions(EditorMap * map, int tilesX, int tilesY)
{
    for (int layerIndex=0; layerIndex<m_selectedMap->layerCount(); ++layerIndex) {
        const EditorMap::MapLayer * layer = m_selectedMap->layer(layerIndex);

        // adjust objects
        for (int i=0; i<layer->objects.size(); ++i) {
            EditorMap::MapObject * object = layer->objects.at(i);
            object->tileX += tilesX;
            object->tileY += tilesY;
        }

        // adjust entities
        for (int i=0; i<layer->entities.size(); ++i) {
            EditorEntity * entity = layer->entities.at(i);
            entity->setCenter(entity->centerX() + tilesX * Tile::sizeInt,
                              entity->centerY() + tilesY * Tile::sizeInt);
        }
    }

    taint();
    this->update();
}

void WorldView::mouseReleaseEvent(QMouseEvent * e)
{
    MouseTool tool = mtNothing;

    if( e->button() == Qt::LeftButton )
        tool = m_toolLeftClick;
    else if( e->button() == Qt::MidButton )
        tool = m_toolMiddleClick;
    else if( e->button() == Qt::RightButton )
        tool = m_toolRightClick;
    else
        return;

    int screenDeltaX = (e->x() - m_mouseDownX);
    int screenDeltaY = (e->y() - m_mouseDownY);
    int deltaX = screenDeltaX * m_zoom;
    int deltaY = screenDeltaY * m_zoom;

    if (m_world == NULL)
        return;

    switch(m_mouseState) {
    case msStretchMapLeft:
        {
            if (m_selectedMap == NULL)
                break;
            int newLeft = snapAbsoluteX(m_selectedMap->left() + deltaX);
            int deltaTilesX = (int) ((m_selectedMap->left() - newLeft) / Tile::size);
            m_selectedMap->addTilesLeft(deltaTilesX);
            adjustItemPositions(m_selectedMap, deltaTilesX, 0);
            taint();
            break;
        }
    case msStretchMapTop:
        {
            if (m_selectedMap == NULL)
                break;

            int newTop = snapAbsoluteY(m_selectedMap->top() + deltaY);
            int deltaTilesY = (int) ((m_selectedMap->top() - newTop) / Tile::size);
            m_selectedMap->addTilesTop(deltaTilesY);
            adjustItemPositions(m_selectedMap, 0, deltaTilesY);

            taint();
            break;
        }
    case msStretchMapRight:
        if (m_selectedMap == NULL)
            break;
        m_selectedMap->setWidth(snapAbsoluteX(m_selectedMap->width() + deltaX));
        taint();
        break;
    case msStretchMapBottom:
        if (m_selectedMap == NULL)
            break;
        m_selectedMap->setHeight(snapAbsoluteY(m_selectedMap->height() + deltaY));
        taint();
        break;
    case msMoveMap:
        if (m_selectedMap == NULL)
            break;
        m_selectedMap->setLeft(snapAbsoluteX(m_selectedMap->left() + deltaX));
        m_selectedMap->setTop(snapAbsoluteY(m_selectedMap->top() + deltaY));
        taint();
        break;
    case msMoveSelectedItems:
        break;
    case msCreateMap:
        {
            QRect dimensions;

            dimensions.setLeft(snapAbsoluteX(absoluteX(m_mouseDownX)));
            dimensions.setTop(snapAbsoluteY(absoluteY(m_mouseDownY)));
            dimensions.setRight(snapAbsoluteX(absoluteX(e->x())));
            dimensions.setBottom(snapAbsoluteY(absoluteY(e->y())));

            // create a new map with these dimensions
            EditorMap * map = EditorMap::createEmpty(
                EditorResourceManager::newUniqueMapName(m_world->name()),
                dimensions.normalized());

            // add to world
            m_world->addMap(map);

            taint();
            updateViewCache();
            break;
        }
    case msSelectionRectangle:
        if (m_selectedMap == NULL)
            break;

        if (! selectItemsInRegion(QRect(m_mouseDownX, m_mouseDownY, screenDeltaX, screenDeltaY))) {
            SelectableItem item = selectableItemAt(m_mouseDownX, m_mouseDownY);
            if (item.isNull()) {
                if (! (e->modifiers() & Qt::ControlModifier))
                    selectNone();
            } else {
                if (e->modifiers() & Qt::ControlModifier)
                    selectAlso(item);
                else
                    selectOnly(item);
            }
        }
        break;
    }

    // return state to normal
    if( tool == m_mouseDownTool ) {
        m_mouseDownTool = mtNothing;
        m_mouseState = msNormal;
        determineCursor();
    }

    updateScrollBars();

    this->update();
}

bool WorldView::selectItemsInRegion(QRect screenRegion)
{
    if (m_selectedMap == NULL)
        return false;

    QRect absRegion = mapRect(screenRegion, m_selectedMap);

    bool anySelected = false;

    for (int layerIndex=0; layerIndex<m_selectedMap->layerCount(); ++layerIndex) {
        const EditorMap::MapLayer * layer = m_selectedMap->layer(layerIndex);

        // check objects
        for (int i=0; i<layer->objects.size(); ++i) {
            EditorMap::MapObject * object = layer->objects.at(i);
            if (absRegion.contains(object->geometry())) {
                selectAlso(object);
                anySelected = true;
            }
        }

        // check entities
        for (int i=0; i<layer->entities.size(); ++i) {
            EditorEntity * entity = layer->entities.at(i);
            if (absRegion.contains(entity->geometry())) {
                selectAlso(entity);
                anySelected = true;
            }
        }
    }

    return anySelected;
}

void WorldView::wheelEvent(QWheelEvent * e)
{
    double degrees = e->delta() / 8;
    double steps = degrees / 15;

    if( e->modifiers() & Qt::ControlModifier ) {
        // zoom
        // TODO
        //m_zoom = Utils::max(Utils::min(m_zoom * std::pow(1.2, steps), 100.0), 0.2);
    } else if( e->orientation() == Qt::Vertical ) {
        m_vsb->setValue((int)(m_vsb->value() - steps * 100 / m_zoom));
    } else if( e->orientation() == Qt::Horizontal ) {
        m_hsb->setValue((int)(m_hsb->value() - steps * 100 / m_zoom));
    }

    this->update();
    e->accept();
}

void WorldView::mousePressEvent(QMouseEvent * e)
{
    // if we are already pressing down the mouse with another tool, return
    if( m_mouseDownTool != mtNothing )
        return;

    MouseTool tool = mtNothing;

    if( e->button() == Qt::LeftButton )
        tool = m_toolLeftClick;
    else if( e->button() == Qt::MidButton )
        tool = m_toolMiddleClick;
    else if( e->button() == Qt::RightButton )
        tool = m_toolRightClick;
    else
        return;

    m_mouseDownX = e->x();
    m_mouseDownY = e->y();
    m_mouseDownTool = tool;

    if (m_world == NULL)
        return;

    switch( tool ){
    case mtNothing:
        break;
    case mtArrow:
        {
            // are we stretching the boundaries of a map?
            if( overMapLeft(e->x(), e->y()) )
                m_mouseState = msStretchMapLeft;
            else if( overMapRight(e->x(), e->y()) )
                m_mouseState = msStretchMapRight;
            else if( overMapTop(e->x(), e->y()) )
                m_mouseState = msStretchMapTop;
            else if( overMapBottom(e->x(), e->y()) )
                m_mouseState = msStretchMapBottom;
            else if( overSelectedMap(e->x(), e->y()) && e->modifiers() & Qt::AltModifier )
                m_mouseState = msMoveMap;
            else {
                // if they clicked inside a map, select it
                selectMap(mapAt(e->x(), e->y()));

                // if they clicked on an already selected object, begin dragging
                SelectableItem item = selectableItemAt(e->x(), e->y());
                if (! item.isNull() && ! e->modifiers() && itemIsSelected(item)) {
                    // begin dragging selection
                    m_mouseState = msMoveSelectedItems;
                    saveSelectionMouseDownCoords();
                } else {
                    // begin making a selection rectangle
                    if (! (e->modifiers() & Qt::ControlModifier))
                        selectNone();

                    m_mouseState = msSelectionRectangle;
                }
            }
            break;
        }
    case mtEraser:
        mouseMoveEvent(e);
        break;
    case mtPan:
        // show hand cursor, and allow dragging the view
        m_mouseDownHScroll = m_hsb->value();
        m_mouseDownVScroll = m_vsb->value();

        m_mouseState = msPan;
        determineCursor();
        break;
    case mtCenter:
        // make the click location the center of the screen
        m_hsb->setValue(absoluteX(e->x() - width() / 2));
        m_vsb->setValue(absoluteY(e->y() - height() / 2));
        updateViewCache();
        break;
    case mtPencil:
        drawSelectedObjectAt(e->x(), e->y());
        break;
    case mtBrush:
        mouseMoveEvent(e);
        break;
    case mtCreateMap:
        m_mouseState = msCreateMap;
        break;
    case mtSetRealStartPoint:
        setStartingPoint(EditorResourceManager::universeFile(), e->x(), e->y());
        break;
    case mtSetTestStartPoint:
        setStartingPoint(EditorResourceManager::testUniverseFile(), e->x(), e->y());
        break;
    default:
        qDebug() << "Invalid tool selected in mousePressEvent";
    }
}

void WorldView::setStartingPoint(QString universeFile, int screenX, int screenY)
{
    EditorUniverse * universe = EditorUniverse::load(universeFile);
    universe->setStartWorld(m_world);
    universe->setStartPosition(absoluteX(screenX), absoluteY(screenY), m_selectedLayer);
    universe->save(universeFile);
    delete universe;

    updateUniverseCache();
}

void WorldView::updateUniverseCache()
{
    // load both playtest and real universe and update variables
    delete m_universe;
    m_universe = QFileInfo(EditorResourceManager::universeFile()).exists() ?
        EditorUniverse::load(EditorResourceManager::universeFile()) : NULL;

    delete m_testUniverse;
    m_testUniverse = QFileInfo(EditorResourceManager::testUniverseFile()).exists() ?
        EditorUniverse::load(EditorResourceManager::testUniverseFile()) : NULL;

    this->update();
}

void WorldView::drawSelectedObjectAt(int x, int y)
{
    // select the map under the cursor
    EditorMap * newMap = mapAt(x,y);

    if (newMap != m_selectedMap)
        selectMap(newMap);

    if (m_selectedMap == NULL)
        return;

    QListWidget * list = m_window->objectsList();
    QListWidgetItem * item = list->currentItem();

    if (item == NULL)
        return;

    // get selected object
    QString file = item->data(Qt::UserRole).toString();
    EditorMap::MapObject * mapObject = new EditorMap::MapObject;
    mapObject->object = EditorObject::load(file);
    mapObject->layer = m_selectedLayer;
    mapObject->tileX = (int) std::floor(mapX(x, m_selectedMap) / Tile::size);
    mapObject->tileY = (int) std::floor(mapY(y, m_selectedMap) / Tile::size);

    // find out if we wouldn't overlap anything by drawing
    bool overlaps = false;
    const EditorMap::MapLayer * layer = m_selectedMap->layer(m_selectedLayer);
    // check objects
    for (int i=0; i<layer->objects.size(); ++i) {
        EditorMap::MapObject * object = layer->objects.at(i);
        if (object->geometry().intersects(mapObject->geometry())) {
            overlaps = true;
            break;
        }
    }

    if (overlaps) {
        delete mapObject;
        return;
    }

    // draw an object at the location
    m_selectedMap->addObject(mapObject);

    taint();
    this->update();
}

bool WorldView::itemIsSelected(const SelectableItem & item)
{
    if (item.isNull())
        return false;

    for (int i=0; i<m_selection.size(); ++i) {
        if (m_selection.at(i)->equals(item))
            return true;
    }
    return false;
}

void WorldView::saveSelectionMouseDownCoords()
{
    for (int i=0; i<m_selection.size(); ++i)
        m_selection.at(i)->saveMouseDownCoords();
}

WorldView::SelectableItem WorldView::selectableItemAt(int x, int y)
{
    if (m_selectedMap == NULL)
        return SelectableItem();

    int absX = mapX(x, m_selectedMap);
    int absY = mapY(y, m_selectedMap);

    // get a list of all items at this point
    QList<SelectableItem> items;
    for (int layerIndex=0; layerIndex<m_selectedMap->layerCount(); ++layerIndex) {
        const EditorMap::MapLayer * layer = m_selectedMap->layer(layerIndex);
        // check objects
        for (int i=0; i<layer->objects.size(); ++i) {
            EditorMap::MapObject * object = layer->objects.at(i);
            if (object->geometry().contains(absX, absY))
                items.append(SelectableItem(object));
        }

        // check entities
        for (int i=0; i<layer->entities.size(); ++i) {
            EditorEntity * entity = layer->entities.at(i);
            if (entity->geometry().contains(absX, absY))
                items.append(SelectableItem(entity));
        }
    }

    // return the smallest one
    if (items.size() == 0) {
        return SelectableItem((EditorEntity *)NULL);
    } else {
        int area = items.at(0).area();
        int index = 0;
        for (int i=1; i<items.size(); ++i) {
            if (items.at(i).area() < area) {
                area = items.at(i).area();
                index = i;
            }
        }

        return items.at(index);
    }
}

EditorMap * WorldView::mapAt(int x, int y)
{
    int absX = absoluteX(x);
    int absY = absoluteY(y);

    for(int i=0; i<m_mapCache.size(); ++i) {
        EditorMap * map = m_mapCache[i];
        if (map->geometry().contains(absX, absY))
            return map;
    }
    return NULL;
}

void WorldView::setWorld(EditorWorld * world)
{
    if (! guiEnsureSaved())
        return;

    selectNone();

    delete m_world;

    m_world = world;
    selectMap(NULL);

    updateScrollBars();

    m_hsb->setValue((int)m_world->left());
    m_vsb->setValue((int)m_world->top());

    updateViewCache();

    m_tainted = false;
    refreshGui();
}

bool WorldView::guiEnsureSaved()
{
    if (m_tainted) {
        QMessageBox::StandardButton result = QMessageBox::question(this,
            QApplication::applicationName(),
            tr("The world %1 has unsaved changes.").arg(m_world->name()),
            QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel,
            QMessageBox::Save);
        if (result == QMessageBox::Cancel)
            return false;
        else if (result == QMessageBox::Save)
            guiSave();
    }

    return true;
}

void WorldView::updateScrollBars()
{
    if (m_world == NULL)
        return;

    const int bufferRoom = 800;

    m_hsb->setMinimum((int)(m_world->left() - bufferRoom));
    m_hsb->setMaximum((int)(m_world->left() + m_world->width()));
    m_vsb->setMinimum((int)(m_world->top() - bufferRoom));
    m_vsb->setMaximum((int)(m_world->top() + m_world->height()));
}

void WorldView::refreshLayersList()
{
    int oldSelectedLayer = m_selectedLayer;

    QListWidget * list = m_window->layersList();
    list->clear();
    if( m_selectedMap ) {
        // add the layers from that map
        for(int i=0; i<m_selectedMap->layerCount(); ++i) {
            QListWidgetItem * newItem = new QListWidgetItem(m_selectedMap->layerName(i), list);
            newItem->setFlags(Qt::ItemIsUserCheckable|Qt::ItemIsSelectable|
                              Qt::ItemIsEditable|Qt::ItemIsEnabled);
            newItem->setCheckState(Qt::Checked);
            list->addItem(newItem);
        }

        if (oldSelectedLayer >= 0 && oldSelectedLayer < m_selectedMap->layerCount())
            m_selectedLayer = oldSelectedLayer;
        else if (m_selectedMap->layerCount() > 0)
            m_selectedLayer = 0;

        list->setCurrentRow(m_selectedLayer);
    } else {
        list->addItem(tr("Click a map to select it and view layers"));
    }
    setControlEnableStates();
}

void WorldView::refreshObjectsList()
{
    QListWidget * list = m_window->objectsList();

    int selectedRow = list->currentRow();

    QDir dir(EditorResourceManager::objectsDir());

    QStringList filters;
    filters << "*.object";

    QStringList entries = dir.entryList(filters, QDir::Files | QDir::Readable,
        QDir::Name | QDir::IgnoreCase);
    list->clear();
    for(int i=0; i<entries.size(); ++i) {
        // create item
        QString file = dir.absoluteFilePath(entries[i]);
        // TODO: create preview icons for objects upon save
        QListWidgetItem * item = new QListWidgetItem(QIcon(), entries[i], list);
        item->setData(Qt::UserRole, QVariant(file));
        list->addItem(item);
    }

    if (selectedRow < list->count())
        list->setCurrentRow(selectedRow);

}

void WorldView::selectMap(EditorMap * map)
{
    m_selectedMap = map;
    refreshGui();
    this->update();
}

void WorldView::verticalScroll(int value)
{
    m_offsetY = value;
    updateViewCache();
}
void WorldView::horizontalScroll(int value)
{
    m_offsetX = value;
    updateViewCache();
}

void WorldView::setSelectedLayer(int index)
{
    m_window->layersList()->setCurrentRow(index);
    m_selectedLayer = index;
    setControlEnableStates();
}

void WorldView::addLayer()
{
    assert(m_selectedMap);
    m_selectedMap->addLayer();
    taint();
    updateViewCache();
    refreshLayersList();
}

void WorldView::swapLayers(int i, int j)
{
    assert(m_selectedMap);
    m_selectedMap->swapLayer(i, j);

    taint();
    updateViewCache();
    refreshLayersList();
}

void WorldView::deleteLayer(int index)
{
    assert(m_selectedMap);
    m_selectedMap->deleteLayer(index);

    taint();
    updateViewCache();
    refreshLayersList();
}


void WorldView::setControlEnableStates()
{
    m_window->moveLayerUpButton()->setEnabled(m_selectedMap != NULL && m_window->layersList()->currentRow() > 0);
    m_window->moveLayerDownButton()->setEnabled(m_selectedMap != NULL && m_window->layersList()->currentRow() < m_window->layersList()->count()-1);
    m_window->deleteLayerButton()->setEnabled(m_selectedMap != NULL && m_window->layersList()->currentRow() > -1);
    m_window->newLayerButton()->setEnabled(m_selectedMap != NULL);

    m_window->newEntityButton()->setEnabled(EditorResourceManager::dataDirIsValid());
    m_window->newObjectButton()->setEnabled(EditorResourceManager::dataDirIsValid());
    m_window->newWorldButton()->setEnabled(EditorResourceManager::dataDirIsValid());
}

void WorldView::setToolLeftClick(MouseTool tool)
{
    m_toolLeftClick = tool;
}

void WorldView::setToolMiddleClick(MouseTool tool)
{
    m_toolMiddleClick = tool;
}

void WorldView::setToolRightClick(MouseTool tool)
{
    m_toolRightClick = tool;
}

void WorldView::dropEvent(QDropEvent * e)
{
    if (m_selectedMap == NULL)
        return;

    if (e->source() == m_window->objectsList()) {
        m_selectedMap->addObject(m_dragObject);
        m_dragObject = NULL;
        this->update();
        e->acceptProposedAction();
    }
}


void WorldView::dragEnterEvent(QDragEnterEvent * e)
{
    if (e->source() == m_window->objectsList()) {
        QString itemModelMime = "application/x-qabstractitemmodeldatalist";

        const QMimeData * data = e->mimeData();
        if(data->hasFormat(itemModelMime)) {
            QStandardItemModel model;
            model.dropMimeData(data, Qt::CopyAction, 0, 0, QModelIndex());
            QString file = model.item(0,0)->data(Qt::UserRole).toString();

            // load the object and set it as the drag object
            m_dragObject = new EditorMap::MapObject;
            m_dragObject->object = EditorObject::load(file);
            m_dragObject->layer = -1;

            e->acceptProposedAction();
        }
    }
}

void WorldView::dragMoveEvent(QDragMoveEvent * e)
{
    EditorMap * dragOverMap = mapAt(e->pos().x(), e->pos().y());

    if (dragOverMap == NULL) {
        m_dragObject->layer = -1;
        e->ignore();
        return; // don't accept event
    }

    if (m_selectedMap != dragOverMap) {
        selectMap(dragOverMap);
        m_dragObject->layer = m_selectedLayer;
    } else if (m_dragObject->layer == -1) {
        m_dragObject->layer = m_selectedLayer;
    }

    m_dragObject->tileX = (int) (snapAbsoluteX(mapX(e->pos().x(), m_selectedMap)) / Tile::size);
    m_dragObject->tileY = (int) (snapAbsoluteY(mapY(e->pos().y(), m_selectedMap)) / Tile::size);
    this->update();

    e->acceptProposedAction();
}

void WorldView::dragLeaveEvent(QDragLeaveEvent * e)
{
    delete m_dragObject->object;
    delete m_dragObject;
    m_dragObject = NULL;
    this->update();
}

void WorldView::guiSave()
{
    if (m_world == NULL)
        return;

    m_world->save();
    m_tainted = false;
    refreshCaption();
}

void WorldView::taint()
{
    m_tainted = true;
    refreshCaption();
}

void WorldView::refreshCaption()
{
    QString tainted = m_tainted ? "*" : "";
    QString dash = " - ";
    QString thisWindow = tr("MotRS World Editor");
    if (m_world == NULL)
        m_window->setWindowTitle(thisWindow);
    else
        m_window->setWindowTitle(m_world->name() + tainted + dash + thisWindow);
}

void WorldView::selectOnly(SelectableItem item)
{
    selectNone();

    // copy the item and append
    SelectableItem * newItem = new SelectableItem();
    *newItem = item;
    m_selection.append(newItem);

    this->update();
}

void WorldView::selectAlso(SelectableItem item)
{
    // copy the item and append
    SelectableItem * newItem = new SelectableItem();
    *newItem = item;
    m_selection.append(newItem);

    this->update();
}

void WorldView::selectNone()
{
    for (int i=0; i<m_selection.size(); ++i)
        delete m_selection.at(i);
    m_selection.clear();
    this->update();
}

void WorldView::deselectItem(SelectableItem & item)
{
    for (int i=0; i<m_selection.size(); ++i) {
        if (m_selection.at(i)->equals(item)) {
            delete m_selection.takeAt(i);
            this->update();
            return;
        }
    }
}

void WorldView::selectAll()
{
    // TODO
}

void WorldView::deleteSelection()
{
    if (m_selectedMap == NULL)
        return;

    for (int i=0; i<m_selection.size(); ++i) {
        SelectableItem * item = m_selection.at(i);
        deleteSelectableItem(*item);
    }

    taint();

    selectNone();
}

void WorldView::copySelection()
{
    // TODO
}

void WorldView::paste()
{
    // TODO
}

void WorldView::deleteSelectableItem(const SelectableItem &item)
{
    if (item.type == sitEditorEntity) {
        m_selectedMap->removeEntity(item.entity);
    } else if (item.type == sitMapObject) {
        m_selectedMap->removeObject(item.object);
    }
}

