#include "WorldView.h"

#include "MainWindow.h"
#include "EditorMap.h"

#include <QPainter>
#include <QDebug>
#include <QSettings>
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
    m_offsetY(0)
{
    m_hsb->show();
    m_vsb->show();

    readSettings();
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
}

void WorldView::paintEvent(QPaintEvent * e)
{
    QPainter p(this);

    p.setBackground(Qt::white);
    p.eraseRect(0, 0, this->width(), this->height());

    if( m_world ) {
        if( m_world->isGood() ) {
            // select the maps that are in range
            std::vector<World::WorldMap> * maps = m_world->maps();
            double viewLeft = absoluteX(0);
            double viewTop = absoluteY(0);
            double viewRight = viewLeft + this->width() * m_zoom;
            double viewBottom = viewTop + this->height() * m_zoom;
            for(unsigned int i=0; i < maps->size(); ++i) {
                // determine if the map is in range
                World::WorldMap * wmap = &(maps->at(i));
                EditorMap * map = (EditorMap *) maps->at(i).map;

                if( !( wmap->x > viewRight || wmap->y > viewBottom ||
                       wmap->x + map->width() < viewLeft || wmap->y + map->height() < viewTop ) )
                {
                    // draw the map
                    // TODO support more layers than 1
                    map->draw(&p, absoluteX(0), absoluteY(0),
                              (double)this->width(), (double)this->height(), 0);
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
                p.drawLine(drawX, 0, drawX, this->height());
                gridX += Tile::size;
            }

            while(gridY < gameBottom) {
                double drawY = screenY(gridY);
                p.drawLine(0, drawY, this->width(), drawY);
                gridY += Tile::size;
            }
        } else if( m_grid == Fast ) {
            for(double y = gridY; y < gameBottom; y+=Tile::size) {
                for(double x = gridX; x < gameRight; x+=Tile::size)
                    p.drawPoint(screenX(x), screenY(y));
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

void WorldView::mousePressEvent(QMouseEvent * e)
{
    switch( m_window->m_mouseState ) {
        case MainWindow::Normal:

            break;
        case MainWindow::SetStartingPoint:
            break;
        default:
            qDebug() << "Unrecognized mouse state: " << m_window->m_mouseState;
    }
}

void WorldView::setWorld(EditorWorld * world)
{
    m_world = world;

    this->update();
}
