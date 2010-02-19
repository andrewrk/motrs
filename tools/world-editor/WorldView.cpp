#include "WorldView.h"

#include "WorldEditor.h"
#include "EditorResourceManager.h"
#include "EditorSettings.h"
#include "EditorGlobals.h"

#include <QPainter>
#include <QDebug>
#include <QListWidget>
#include <QList>
#include <QUrl>
#include <QFileInfo>
#include <QMessageBox>
#include <QDir>

#include <cmath>

#include "moc_WorldView.cxx"

QPainter * WorldView::s_painter = NULL;

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
    m_mouseDownTool(Nothing),
    m_mouseState(Normal)
{
    m_hsb->show();
    m_vsb->show();

    connect(m_vsb, SIGNAL(valueChanged(int)), this, SLOT(verticalScroll(int)));
    connect(m_hsb, SIGNAL(valueChanged(int)), this, SLOT(horizontalScroll(int)));

    updateViewCache();

    this->setMouseTracking(true);
    this->setAcceptDrops(true);

}

WorldView::~WorldView()
{
    if( m_world )
        delete m_world;
}

void WorldView::refreshGui()
{
    refreshLayersList();
    refreshObjectsList();
    setControlEnableStates();
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
    m_entityCache.clear();
    if( m_world && m_world->isGood() ) {
        // select the maps that are in range
        std::vector<Map*> * maps = m_world->maps();
        double viewLeft = absoluteX(0);
        double viewTop = absoluteY(0);
        double viewRight = viewLeft + this->width() * m_zoom;
        double viewBottom = viewTop + this->height() * m_zoom;
        m_maxLayer = 0;
        for(unsigned int i=0; i < maps->size(); ++i) {
            // determine if the map is in range
            EditorMap * map = (EditorMap *) maps->at(i);

            if (!(map->left() > viewRight || map->top() > viewBottom ||
                  map->left() + map->width() < viewLeft || map->top() + map->height() < viewTop))
            {
                if (map->layerCount() > m_maxLayer)
                    m_maxLayer = map->layerCount();

                m_mapCache.append(map);

                // add the map's entities to our cache
                for(unsigned int j = 0; j<map->entities()->size(); ++j)
                    m_entityCache.append( (EditorEntity *) map->entities()->at(j));
            }
        }
    }

    this->update();
}

void WorldView::paintEvent(QPaintEvent * e)
{
    QPainter p(this);
    s_painter = &p;
    p.setBackground(Qt::white);
    p.eraseRect(0, 0, this->width(), this->height());

    if( m_world ) {
        if( m_world->isGood() ) {
            double absX = absoluteX(0);
            double absY = absoluteY(0);

            for(int layer=0; layer<m_maxLayer; ++layer) {
                // draw map at this layer
                for(int i=0; i<m_mapCache.size(); ++i) {
                    EditorMap * map = m_mapCache[i];

                    // if the map is selected and this layer is unchecked, don't draw
                    if( map == m_selectedMap && m_window->layersList()->item(layer)->checkState() == Qt::Unchecked)
                        continue;

                    if( layer < map->layerCount() )
                        map->draw(absX, absY,
                            (double)this->width(), (double)this->height(), layer);
                }

                // draw entities at this layer
                for(int i=0; i<m_entityCache.size(); ++i) {
                    if( m_entityCache[i]->layer() == layer )
                        m_entityCache[i]->draw(absX, absY);
                }
            }
            // draw a bold line around map borders
            QPen normalMapBorder(Qt::black, 2);
            QPen selectedMapBorder(Qt::blue, 2);
            p.setBrush(Qt::NoBrush);
            for (int i = 0; i < m_mapCache.size(); i++) {
                EditorMap * map = m_mapCache[i];

                p.setPen(m_selectedMap == map ? selectedMapBorder : normalMapBorder);
                QRect outline((int)screenX(map->left()), (int)screenY(map->top()),
                    (int)(map->width() * m_zoom), (int)(map->height() * m_zoom));

                // if we're dragging boundaries, move the blue line
                if( m_selectedMap == map ) {
                    int deltaX = m_mouseX - m_mouseDownX;
                    int deltaY = m_mouseY - m_mouseDownY;
                    if (m_mouseState == StretchMapLeft)
                        outline.setLeft(outline.left() + deltaX);
                    else if (m_mouseState == StretchMapBottom)
                        outline.setBottom(outline.bottom() + deltaY);
                    else if (m_mouseState == StretchMapRight)
                        outline.setRight(outline.right() + deltaX);
                    else if (m_mouseState == StretchMapTop)
                        outline.setTop(outline.top() + deltaY);
                    else if (m_mouseState == MoveMap)
                        outline.moveTo(outline.left() + deltaX, outline.top() + deltaY);

                }

                p.drawRect(outline);
            }

            drawGrid(p);
        } else {
            p.drawText(0, 0, this->width(), this->height(), Qt::AlignCenter,
                tr("Error loading World."));
        }
    } else {
        p.drawText(0, 0, this->width(), this->height(), Qt::AlignCenter,
            tr("Double click a world to edit"));
    }

    s_painter = NULL;
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

        double gameLeft = absoluteX(0);
        double gameTop = absoluteY(0);
        double gameRight = absoluteX(this->width());
        double gameBottom = absoluteY(this->height());
        double gridX = gameLeft - std::fmod(gameLeft, Tile::size);
        double gridY = gameTop - std::fmod(gameTop, Tile::size);

        if( gridType == EditorSettings::Pretty || gridType == EditorSettings::Solid ) {
            while(gridX < gameRight) {
                double drawX = screenX(gridX);
                p.drawLine((int)drawX, 0, (int)drawX, this->height());
                gridX += Tile::size;
            }

            while(gridY < gameBottom) {
                double drawY = screenY(gridY);
                p.drawLine(0, (int)drawY, this->width(), (int)drawY);
                gridY += Tile::size;
            }
        } else if( gridType == EditorSettings::Fast ) {
            for(double y = gridY; y < gameBottom; y+=Tile::size) {
                for(double x = gridX; x < gameRight; x+=Tile::size)
                    p.drawPoint((int)screenX(x), (int)screenY(y));
            }
        }
    }
}

double WorldView::screenX(double absoluteX)
{
    return (absoluteX - m_offsetX) * m_zoom;
}

double WorldView::screenY(double absoluteY)
{
    return (absoluteY - m_offsetY) * m_zoom;
}

double WorldView::absoluteX(double screenX)
{
    return (screenX / m_zoom) + m_offsetX;
}

double WorldView::absoluteY(double screenY)
{
    return (screenY / m_zoom) + m_offsetY;
}

bool WorldView::overMapLeft(int x, int y)
{
    if( ! m_selectedMap )
        return false;

    double absX = absoluteX(x);
    double absY = absoluteY(y);

    return  absX > m_selectedMap->left() - g_lineSelectRadius &&
            absX < m_selectedMap->left() + g_lineSelectRadius &&
            absY > m_selectedMap->top() && absY < m_selectedMap->top() + m_selectedMap->height();
}

bool WorldView::overMapTop(int x, int y)
{
    if( ! m_selectedMap )
        return false;

    double absX = absoluteX(x);
    double absY = absoluteY(y);

    return  absX > m_selectedMap->left() && absX < m_selectedMap->left() + m_selectedMap->width() &&
            absY > m_selectedMap->top() - g_lineSelectRadius &&
            absY < m_selectedMap->top() + g_lineSelectRadius;
}
bool WorldView::overMapRight(int x, int y)
{
    if( ! m_selectedMap )
        return false;

    double absX = absoluteX(x);
    double absY = absoluteY(y);

    return  absX > m_selectedMap->left() + m_selectedMap->width() - g_lineSelectRadius &&
            absX < m_selectedMap->left() + m_selectedMap->width() + g_lineSelectRadius &&
            absY > m_selectedMap->top() && absY < m_selectedMap->top() + m_selectedMap->height();

}
bool WorldView::overMapBottom(int x, int y)
{
    if( ! m_selectedMap )
        return false;

    double absX = absoluteX(x);
    double absY = absoluteY(y);

    return  absX > m_selectedMap->left() && absX < m_selectedMap->left() + m_selectedMap->width() &&
            absY > m_selectedMap->top() + m_selectedMap->height() - g_lineSelectRadius &&
            absY < m_selectedMap->top() + m_selectedMap->height() + g_lineSelectRadius;
}

bool WorldView::overSelectedMap(int x, int y)
{
    if( ! m_selectedMap )
        return false;

    double absX = absoluteX(x);
    double absY = absoluteY(y);

    return  absX > m_selectedMap->left() && absX < m_selectedMap->left() + m_selectedMap->width() &&
            absY > m_selectedMap->top() && absY < m_selectedMap->top() + m_selectedMap->height();
}


void WorldView::keyPressEvent(QKeyEvent * e)
{
    m_keyboardModifiers = e->modifiers();
    determineCursor();
}

void WorldView::keyReleaseEvent(QKeyEvent * e)
{
    m_keyboardModifiers = e->modifiers();
    determineCursor();
}

void WorldView::determineCursor()
{
    switch(m_mouseState) {
        case Normal: {
            switch(m_mouseDownTool) {
                case Nothing: {
                    // change mouse cursor to sizers if over map boundaries
                    // if the user could use the arrow tool
                    if( m_toolLeftClick == Arrow ||
                        m_toolMiddleClick == Arrow ||
                        m_toolRightClick == Arrow )
                    {
                        if( overMapLeft(m_mouseX, m_mouseY) || overMapRight(m_mouseX, m_mouseY))
                            this->setCursor(Qt::SizeHorCursor);
                        else if( overMapTop(m_mouseX, m_mouseY) || overMapBottom(m_mouseX, m_mouseY))
                            this->setCursor(Qt::SizeVerCursor);
                        else if( overSelectedMap(m_mouseX, m_mouseY) && m_keyboardModifiers & Qt::ControlModifier)
                            this->setCursor(Qt::SizeAllCursor);
                        else
                            this->setCursor(Qt::ArrowCursor);
                    }
                } break;
                case Arrow: break;
                case Eraser: break;
                case Pan: break;
                case Center: break;
                case Pencil: break;
                case Brush: break;
            }
        } break;
        case SetStartPoint: break;
        case StretchMapLeft: break;
        case StretchMapTop: break;
        case StretchMapRight: break;
        case StretchMapBottom: break;
        case MoveMap: break;
    }
}

void WorldView::mouseMoveEvent(QMouseEvent * e)
{
    m_mouseX = e->x();
    m_mouseY = e->y();
    m_keyboardModifiers = e->modifiers();

    determineCursor();
    switch(m_mouseState) {
        case Normal: {
            switch(m_mouseDownTool) {
                case Nothing: break;
                case Arrow: break;
                case Eraser: break;
                case Pan: break;
                case Center: break;
                case Pencil: break;
                case Brush: break;
            }
        } break;
        case SetStartPoint: break;

        // update the screen when we're making a stretch box
        case StretchMapLeft:
        case StretchMapTop:
        case StretchMapRight:
        case StretchMapBottom:
        case MoveMap:
            this->update();
            break;
    }
}

void WorldView::mouseReleaseEvent(QMouseEvent * e)
{
    MouseTool tool = Nothing;

    if( e->button() == Qt::LeftButton )
        tool = m_toolLeftClick;
    else if( e->button() == Qt::MidButton )
        tool = m_toolMiddleClick;
    else if( e->button() == Qt::RightButton )
        tool = m_toolRightClick;
    else
        return;

    double deltaX = (e->x() - m_mouseDownX) * m_zoom;
    double deltaY = (e->y() - m_mouseDownY) * m_zoom;
    switch(m_mouseState) {
        case StretchMapLeft:
            m_selectedMap->setLeft(m_selectedMap->left() + deltaX);
            m_selectedMap->addTilesLeft((int)(-deltaX / Tile::size));
        break;
        case StretchMapTop:this->setCursor(Qt::ArrowCursor);
            m_selectedMap->setTop(m_selectedMap->top() + deltaY);
            m_selectedMap->addTilesTop((int)(-deltaY / Tile::size));
        break;
        case StretchMapRight:
            m_selectedMap->setWidth(m_selectedMap->width() + deltaX);
        break;
        case StretchMapBottom:
            m_selectedMap->setHeight(m_selectedMap->height() + deltaY);
        break;
        case MoveMap:
            m_selectedMap->setLeft(m_selectedMap->left() + deltaX);
            m_selectedMap->setTop(m_selectedMap->top() + deltaY);
        break;
    }

    // return state to normal
    if( tool == m_mouseDownTool ) {
        m_mouseDownTool = Nothing;
        m_mouseState = Normal;
        determineCursor();
    }

    this->update();
}

void WorldView::mousePressEvent(QMouseEvent * e)
{
    // if we are already pressing down the mouse with another tool, return
    if( m_mouseDownTool != Nothing )
        return;

    MouseTool tool = Nothing;

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

    switch( tool ){
        case Nothing:
            break;
        case Arrow: {
            // are we stretching the boundaries of a map?
            if( overMapLeft(e->x(), e->y()) )
                m_mouseState = StretchMapLeft;
            else if( overMapRight(e->x(), e->y()) )
                m_mouseState = StretchMapRight;
            else if( overMapTop(e->x(), e->y()) )
                m_mouseState = StretchMapTop;
            else if( overMapBottom(e->x(), e->y()) )
                m_mouseState = StretchMapBottom;
            else if( overSelectedMap(e->x(), e->y()) && e->modifiers() & Qt::ControlModifier )
                m_mouseState = MoveMap;
            else
                selectMap(mapAt(e->x(), e->y())); // if they clicked inside a map, select it
        } break;
        case Eraser:

            break;
        case Pan:

            break;
        case Center:

            break;
        case Pencil:

            break;
        case Brush:

            break;
        default:
            qDebug() << "Invalid tool selected in mousePressEvent";
    }
}

EditorMap * WorldView::mapAt(int x, int y)
{
    double absX = absoluteX(x);
    double absY = absoluteY(y);
    for(int i=0; i<m_mapCache.size(); ++i) {
        EditorMap * map = m_mapCache[i];
        if( absX > map->left() && absX < map->left() + map->width() &&
            absY > map->top() && absY < map->top() + map->height() )
        {
            return map;
        }
    }
    return NULL;
}

void WorldView::setWorld(EditorWorld * world)
{
    m_world = world;
    selectMap(NULL);

    // set up scroll bars
    const int bufferRoom = 800;
    m_hsb->setMinimum((int)(m_world->left() - bufferRoom));
    m_hsb->setMaximum((int)(m_world->left() + m_world->width()));
    m_hsb->setValue((int)m_world->left());
    m_vsb->setMinimum((int)(m_world->top() - bufferRoom));
    m_vsb->setMaximum((int)(m_world->top() + m_world->height()));
    m_vsb->setValue((int)(m_world->top()));

    updateViewCache();

    refreshGui();
}

void WorldView::refreshLayersList()
{
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

        if( m_selectedMap->layerCount() > 0 )
            list->item(0)->setSelected(true);
    } else {
        list->addItem(tr("Click a map to select it and view layers"));
    }
    setControlEnableStates();
}

void WorldView::refreshObjectsList()
{
    QListWidget * list = m_window->objectsList();

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
    Debug::assert(m_selectedMap, "Error: Layer manipulations on no selected map");
    m_selectedMap->addLayer();

    updateViewCache();
    refreshLayersList();
}

void WorldView::swapLayers(int i, int j)
{
    Debug::assert(m_selectedMap, "Error: Layer manipulations on no selected map");
    m_selectedMap->swapLayer(i, j);

    updateViewCache();
    refreshLayersList();
}

void WorldView::deleteLayer(int index)
{
    Debug::assert(m_selectedMap, "Error: Layer manipulations on no selected map");
    m_selectedMap->deleteLayer(index);
    updateViewCache();
    refreshLayersList();
}


void WorldView::setControlEnableStates()
{
    m_window->moveLayerUpButton()->setEnabled(m_selectedMap != NULL && m_window->layersList()->currentRow() > 0);
    m_window->moveLayerDownButton()->setEnabled(m_selectedMap != NULL && m_window->layersList()->currentRow() < m_window->layersList()->count()-1);
    m_window->deleteLayerButton()->setEnabled(m_selectedMap != NULL && m_window->layersList()->currentRow() > -1);
    m_window->newLayerButton()->setEnabled(m_selectedMap != NULL);

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
