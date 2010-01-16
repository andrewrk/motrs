#include "worldview.h"

#include <QPainter>

#include "moc_worldview.cxx"

WorldView::WorldView(QWidget * parent) :
    QWidget(parent),
    m_hsb(new QScrollBar(Qt::Horizontal, this)),
    m_vsb(new QScrollBar(Qt::Vertical, this))
{
    m_hsb->show();
    m_vsb->show();
}

WorldView::~WorldView()
{

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

    p.setBackground(QBrush(Qt::black));
    p.eraseRect(0, 0, this->width(), this->height());
}
