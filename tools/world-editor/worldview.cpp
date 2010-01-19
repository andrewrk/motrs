#include "worldview.h"

#include "mainwindow.h"
#include <QPainter>
#include <QDebug>
#include <cmath>

#include "moc_worldview.cxx"

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
}

WorldView::~WorldView()
{
    if( m_world )
        delete m_world;
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

    p.setBackground(QBrush(Qt::white));
    p.eraseRect(0, 0, this->width(), this->height());


    if( m_world ) {
        // select the tiles that are in range
        // determine what tile the top left corner is
        Universe::Location loc = m_world->locationOf(absoluteX(0), absoluteY(0));
        double cursorX = loc.x;
        while(screenX(cursorX) < this->width()) {
            double cursorY = loc.y;
            while(screenY(cursorY) < this->height()) {
                Universe::Location thisLoc = m_world->locationOf(cursorX, cursorY);

                int flooredX = std::floor(cursorX);
                int flooredY = std::floor(cursorY);
                double drawX = screenX(flooredX - (flooredX % Tile::sizeInt));
                double drawY = screenY(flooredY - (flooredY % Tile::sizeInt));

                if( thisLoc.map == NULL ) {
                    // no Map covers this tile
                    p.setBrush(QBrush(Qt::black, Qt::SolidPattern));
                    p.drawRect(drawX, drawY, Tile::size, Tile::size);
                } else {
                    // draw tile

                    // draw grid box
                    p.setBrush(QBrush(Qt::black, Qt::NoBrush));
                    p.drawRect(drawX, drawY, Tile::size, Tile::size);
                }

                cursorY += Tile::size;
            }
            cursorX += Tile::size;
        }
    } else {
        p.drawText(0, 0, this->width(), this->height(), Qt::AlignCenter,
            tr("Double click a world to edit"));
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
