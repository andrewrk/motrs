#include "WorldView.h"

#include "MainWindow.h"

#include <QPainter>
#include <QDebug>
#include <QSettings>
#include <QListWidget>
#include <cmath>

#include "moc_WorldView.cxx"

WorldView::WorldView(MainWindow * window, QWidget * parent) :
    QWidget(parent),
    m_hsb(new QScrollBar(Qt::Horizontal, this)),
    m_vsb(new QScrollBar(Qt::Vertical, this)),
    m_window(window),
    m_world(NULL),
    m_zoom(1.0),
    m_offsetX(0),
    m_offsetY(0),
    m_mapCache(),
    m_selectedMap(NULL)
{
    m_hsb->show();
    m_vsb->show();

    connect(m_vsb, SIGNAL(valueChanged(int)), this, SLOT(verticalScroll(int)));
    connect(m_hsb, SIGNAL(valueChanged(int)), this, SLOT(horizontalScroll(int)));

    readSettings();
    updateViewCache();
}

WorldView::~WorldView()
{
    if( m_world )
        delete m_world;
}

void WorldView::readSettings()
{

    QSettings settings;
    m_grid = (GridRenderType)settings.value("editor/grid", Pretty).toInt();

    this->update();
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

    if( m_world ) {
        if( m_world->isGood() ) {
            for(int layer=0; layer<m_maxLayer; ++layer) {
                for(int i=0; i<m_mapCache.size(); ++i) {
                    if( layer < m_mapCache[i]->layerCount() )
                        m_mapCache[i]->draw(&p, absoluteX(0), absoluteY(0),
                            (double)this->width(), (double)this->height(), layer);
                }
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
}

void WorldView::drawGrid(QPainter &p)
{
    if( m_grid != None ) {
        if( m_grid == Pretty )
            p.setPen(QColor(128, 128, 128, 64));
        else
            p.setPen(QColor(128, 128, 128));

        // vertical lines
        double gameLeft = absoluteX(0);
        double gameTop = absoluteY(0);
        double gameRight = absoluteX(this->width());
        double gameBottom = absoluteY(this->height());
        double gridX = gameLeft - std::fmod(gameLeft, Tile::size);
        double gridY = gameTop - std::fmod(gameTop, Tile::size);

        if( m_grid == Pretty ) {
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
        } else if( m_grid == Fast ) {
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

void WorldView::mouseMoveEvent(QMouseEvent * e)
{

}

void WorldView::mousePressEvent(QMouseEvent * e)
{
    MainWindow::MouseTool tool = MainWindow::Nothing;
    MainWindow * parent = (MainWindow *) this->parent();

    if( e->button() == Qt::LeftButton )
        tool = parent->m_toolLeftClick;
    else if( e->button() == Qt::MidButton )
        tool = parent->m_toolMiddleClick;
    else if( e->button() == Qt::RightButton )
        tool = parent->m_toolRightClick;

    switch( tool ){
        case MainWindow::Nothing:
            break;
        case MainWindow::Arrow:
            // if they clicked inside a map, select it
            selectMap(mapAt(e->x(), e->y()));
            break;
        case MainWindow::Eraser:

            break;
        case MainWindow::Pan:

            break;
        case MainWindow::Center:

            break;
        case MainWindow::Pencil:

            break;
        case MainWindow::Brush:

            break;
        case MainWindow::SetStartingPoint:

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

    updateViewCache();
}

void WorldView::selectMap(EditorMap * map)
{
    m_selectedMap = map;
    MainWindow * parent = (MainWindow *) this->parent();
    QListWidget * list = parent->layersList();
    list->clear();
    if( m_selectedMap ) {
        // add the layers from that map
        for(int i=0; i<m_selectedMap->layerCount(); ++i){
            QListWidgetItem * newItem = new QListWidgetItem(tr("Layer %1").arg(i+1), list);
            newItem->setFlags(Qt::ItemIsUserCheckable);
            newItem->setCheckState(Qt::Checked);
            list->addItem(newItem);
        }
    } else {
        list->addItem(tr("Click a map to select it and view layers"));
    }
}

void WorldView::verticalScroll(int value)
{
    qDebug() << value ;
}
void WorldView::horizontalScroll(int value)
{
    qDebug() << value;
}
