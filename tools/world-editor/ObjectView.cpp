#include "ObjectView.h"

#include "EditorResourceManager.h"
#include "ObjectEditor.h"
#include "EditorSettings.h"
#include "Utils.h"

#include <QPainter>
#include <QStandardItemModel>
#include <QFileInfo>
#include <QDebug>
#include <QMessageBox>

#include <cmath>

ObjectView::ObjectView(ObjectEditor * window, QWidget * parent) :
    QWidget(parent),
    m_window(window),
    m_object(new EditorMap()),
    m_dragPixmap(NULL),
    m_btnTopPlus(new QPushButton("+", this)),
    m_btnTopMinus(new QPushButton("-", this)),
    m_btnBottomPlus(new QPushButton("+", this)),
    m_btnBottomMinus(new QPushButton("-", this)),
    m_btnLeftPlus(new QPushButton("+", this)),
    m_btnLeftMinus(new QPushButton("-", this)),
    m_btnRightPlus(new QPushButton("+", this)),
    m_btnRightMinus(new QPushButton("-", this)),
    m_hsb(new QScrollBar(Qt::Horizontal, this)),
    m_vsb(new QScrollBar(Qt::Vertical, this)),
    m_selectedLayer(-1),
    m_viewMode(Normal),
    m_zoom(1.0),
    m_offsetX(0),
    m_offsetY(0)
{
    m_btnTopPlus->show();
    m_btnTopMinus->show();
    m_btnBottomPlus->show();
    m_btnBottomMinus->show();
    m_btnLeftPlus->show();
    m_btnLeftMinus->show();
    m_btnRightPlus->show();
    m_btnRightMinus->show();

    m_hsb->show();
    m_vsb->show();
}

ObjectView::~ObjectView()
{
    if( m_object )
        delete m_object;
}

void ObjectView::paintEvent(QPaintEvent * e)
{
    QPainter p(this);

    // clear the screen
    p.setBackground(Qt::white);
    p.eraseRect(0, 0, this->width(), this->height());

    for(int layer=0; layer<m_object->layerCount(); ++layer) {
        // draw all art items at this layer

        // draw dragged stuff
        if( layer == m_selectedLayer && m_dragPixmap != NULL ) {
            p.drawPixmap(m_dragPixmapX, m_dragPixmapY, *m_dragPixmap);
        }
    }

    drawGrid(p);
}

void ObjectView::dropEvent(QDropEvent * e)
{
    QStringList pictureExtensions;
    pictureExtensions << "bmp" << "jpg" << "png";

    QString itemModelMime = "application/x-qabstractitemmodeldatalist";

    // don't show the art in paintevent
    m_dragPixmap = NULL;
    const QMimeData * data = e->mimeData();
    if(data->hasFormat(itemModelMime)) {
        QStandardItemModel model;
        model.dropMimeData(data, Qt::CopyAction, 0, 0, QModelIndex());
        QString file = model.item(0,0)->data(Qt::UserRole).toString();

        QFileInfo info(file);
        QString ext = info.suffix();
        if( pictureExtensions.contains(ext, Qt::CaseInsensitive) ) {
            // it's art
            QString title = info.fileName();
            QPixmap * pixmap = EditorResourceManager::pixmapForArt(title);

        } else if( ext.compare("entity", Qt::CaseInsensitive) ) {
            // it's an entity
            // TODO
        } else if( ext.compare("map", Qt::CaseInsensitive) ) {
            // it's a map
            // TODO
        } else {
            qDebug() << "Unable to drag drop extension " << ext;
            QMessageBox::warning(this, tr("error"), tr("Uh oh, we don't support %1 yet.").arg(ext));
            return;
        }
        e->acceptProposedAction();
    }
}


void ObjectView::dragEnterEvent(QDragEnterEvent * e)
{
    if( e->source() == m_window->artList() ) {
        QStringList pictureExtensions;
        pictureExtensions << "bmp" << "jpg" << "png";

        QString itemModelMime = "application/x-qabstractitemmodeldatalist";

        const QMimeData * data = e->mimeData();
        if(data->hasFormat(itemModelMime)) {
            QStandardItemModel model;
            model.dropMimeData(data, Qt::CopyAction, 0, 0, QModelIndex());
            QString file = model.item(0,0)->data(Qt::UserRole).toString();

            QFileInfo info(file);
            QString ext = info.suffix();
            if( pictureExtensions.contains(ext, Qt::CaseInsensitive) ) {
                // it's art
                QString title = info.fileName();
                qDebug() << title;
                QPixmap * pixmap = EditorResourceManager::pixmapForArt(title);
                m_dragPixmap = pixmap;
            } else if( ext.compare("entity", Qt::CaseInsensitive) ) {
                // it's an entity
                // TODO
            } else if( ext.compare("map", Qt::CaseInsensitive) ) {
                // it's a map
                // TODO
            } else {
                qDebug() << "Unable to drag drop extension " << ext;
                QMessageBox::warning(this, tr("error"), tr("Uh oh, we don't support %1 yet.").arg(ext));
                return;
            }
            e->acceptProposedAction();
        }
    }
}

void ObjectView::dragMoveEvent(QDragMoveEvent * e)
{
    m_dragPixmapX = e->pos().x();
    m_dragPixmapY = e->pos().y();
    this->update();

    e->acceptProposedAction();
}

void ObjectView::dragLeaveEvent(QDragLeaveEvent * e)
{
    m_dragPixmap = NULL;
}

void ObjectView::resizeEvent(QResizeEvent * e)
{
    // move the scroll bars and +/- buttons into position
    int sqWidth = m_vsb->width();
    int sqHeight = m_hsb->height();
    int bottomLine = this->height()-sqHeight;
    int rightLine = this->width()-sqWidth;

    m_btnLeftPlus->setGeometry(0, bottomLine, sqWidth, sqHeight);
    m_btnLeftMinus->setGeometry(sqWidth, bottomLine, sqWidth, sqHeight);
    m_btnRightPlus->setGeometry(rightLine-sqWidth, bottomLine, sqWidth, sqHeight);
    m_btnRightMinus->setGeometry(rightLine-sqWidth*2, bottomLine, sqWidth, sqHeight);
    m_btnTopPlus->setGeometry(rightLine, 0, sqWidth, sqHeight);
    m_btnTopMinus->setGeometry(rightLine, sqHeight, sqWidth, sqHeight);
    m_btnBottomPlus->setGeometry(rightLine, bottomLine-sqHeight, sqWidth, sqHeight);
    m_btnBottomMinus->setGeometry(rightLine, bottomLine-sqHeight*2, sqWidth, sqHeight);

    m_hsb->setGeometry(sqWidth*2, bottomLine, this->width()-sqWidth*5, sqHeight);
    m_vsb->setGeometry(rightLine, sqHeight*2, sqWidth, this->height()-sqHeight*5);

    update();
}

void ObjectView::setSelectedLayer(int layer)
{
    m_selectedLayer = layer;
}

void ObjectView::open(QString file)
{
    if( m_object )
        delete m_object;
    m_object = new EditorMap(file);
    // TODO: if (! m_object.isGood) delete m_object; m_object = NULL;
    refreshLayersList();
    update();
}

void ObjectView::refreshLayersList()
{
    // TODO
}

void ObjectView::setViewMode(ViewMode mode)
{
    m_viewMode = mode;
    update();
}

void ObjectView::drawGrid(QPainter &p)
{
    EditorSettings::GridRenderType gridType = EditorSettings::gridRenderType();
    if( gridType != EditorSettings::None ) {
        if( gridType == EditorSettings::Pretty )
            p.setPen(QColor(128, 128, 128, 64));
        else
            p.setPen(QColor(128, 128, 128));

        double gameLeft = absoluteX(0);
        double gameTop = absoluteY(0);
        double gameRight = Utils::min(absoluteX(this->width()), absoluteX(m_object->width())) + 0.1; // add 0.1 to make up for double precision
        double gameBottom = Utils::min(absoluteY(this->height()), absoluteY(m_object->height())) + 0.1;
        double gridX = gameLeft - std::fmod(gameLeft, Tile::size);
        double gridY = gameTop - std::fmod(gameTop, Tile::size);

        if( gridType == EditorSettings::Pretty ) {
            while(gridX < gameRight) {
                double drawX = screenX(gridX);
                p.drawLine((int)drawX, 0, (int)drawX, screenX(gameRight));
                gridX += Tile::size;
            }

            while(gridY < gameBottom) {
                double drawY = screenY(gridY);
                p.drawLine(0, (int)drawY, screenY(gameBottom), (int)drawY);
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

double ObjectView::screenX(double absoluteX)
{
    return (absoluteX - m_offsetX) * m_zoom;
}

double ObjectView::screenY(double absoluteY)
{
    return (absoluteY - m_offsetY) * m_zoom;
}

double ObjectView::absoluteX(double screenX)
{
    return (screenX / m_zoom) + m_offsetX;
}

double ObjectView::absoluteY(double screenY)
{
    return (screenY / m_zoom) + m_offsetY;
}
