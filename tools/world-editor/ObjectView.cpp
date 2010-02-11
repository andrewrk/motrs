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
#include <QTableWidget>

#include <cmath>

#include "moc_ObjectView.cxx"

ObjectView::ObjectView(ObjectEditor * window, QWidget * parent) :
    QWidget(parent),
    m_window(window),
    m_object(NULL),
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

    connect(m_vsb, SIGNAL(valueChanged(int)), this, SLOT(verticalScroll(int)));
    connect(m_hsb, SIGNAL(valueChanged(int)), this, SLOT(horizontalScroll(int)));
    connect(m_btnTopPlus, SIGNAL(clicked()), this, SLOT(on_btnTopPlus_clicked()));
    connect(m_btnTopMinus, SIGNAL(clicked()), this, SLOT(on_btnTopMinus_clicked()));
    connect(m_btnBottomPlus, SIGNAL(clicked()), this, SLOT(on_btnBottomPlus_clicked()));
    connect(m_btnBottomMinus, SIGNAL(clicked()), this, SLOT(on_btnBottomMinus_clicked()));
    connect(m_btnLeftPlus, SIGNAL(clicked()), this, SLOT(on_btnLeftPlus_clicked()));
    connect(m_btnLeftMinus, SIGNAL(clicked()), this, SLOT(on_btnLeftMinus_clicked()));
    connect(m_btnRightPlus, SIGNAL(clicked()), this, SLOT(on_btnRightPlus_clicked()));
    connect(m_btnRightMinus, SIGNAL(clicked()), this, SLOT(on_btnRightMinus_clicked()));
}

ObjectView::~ObjectView()
{
    if( m_object )
        delete m_object;
}

void ObjectView::createEmpty()
{
    if( m_object )
        delete m_object;
    m_object = new EditorMap();

    m_objectName = QObject::tr("New Object");
    refreshGui();
}

void ObjectView::refreshGui()
{
    refreshLayersList();
    refreshProperties();
    setUpScrolling();
    this->update();
}

void ObjectView::paintEvent(QPaintEvent * e)
{
    QPainter p(this);

    // clear the screen
    p.setBackground(Qt::white);
    p.eraseRect(0, 0, this->width(), this->height());

    if( m_object ) {
        for(int layer=0; layer<m_object->layerCount(); ++layer) {
            // draw all art items at this layer

            // draw dragged stuff
            if( layer == m_selectedLayer && m_dragPixmap != NULL ) {
                p.drawPixmap(m_dragPixmapX, m_dragPixmapY, *m_dragPixmap);
            }
        }

        drawGrid(p);
    } else {
        p.drawText(0, 0, this->width(), this->height(), Qt::AlignCenter,
            tr("No object loaded"));
    }
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
    setControlEnableStates();
}

void ObjectView::open(QString file)
{
    if( m_object )
        delete m_object;
    m_object = new EditorMap(file);
    // TODO: if (! m_object.isGood) delete m_object; m_object = NULL;

    m_objectName = file;
    refreshGui();
}

void ObjectView::refreshLayersList()
{
    QListWidget * list = m_window->layersList();
    list->clear();
    if( m_object ) {
        // add the layers from that map
        for(int i=0; i<m_object->layerCount(); ++i) {
            QListWidgetItem * newItem = new QListWidgetItem(m_object->layerName(i), list);
            newItem->setFlags(Qt::ItemIsUserCheckable|Qt::ItemIsSelectable|
                              Qt::ItemIsEditable|Qt::ItemIsEnabled);
            newItem->setCheckState(Qt::Checked);
            list->addItem(newItem);
        }

        if( m_object->layerCount() > 0 ) {
            list->item(0)->setSelected(true);
            m_selectedLayer = 0;
        } else {
            m_selectedLayer = -1;
        }
    } else {
        list->addItem(tr("No object loaded"));
    }
    setControlEnableStates();
}

void ObjectView::setControlEnableStates()
{
    m_window->moveLayerUpButton()->setEnabled(m_object != NULL && m_selectedLayer > 0);
    m_window->moveLayerDownButton()->setEnabled(m_object != NULL && m_selectedLayer < m_object->layerCount()-1);
    m_window->deleteLayerButton()->setEnabled(m_object != NULL && m_selectedLayer > -1);
    m_window->newLayerButton()->setEnabled(m_object != NULL);
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
        int sizeX = m_object->width() / Tile::size;
        int sizeY = m_object->height() / Tile::size;

        if( gridType == EditorSettings::Pretty ) {
            p.setPen(QColor(128, 128, 128, 64));

            // vertical
            for(int x=0; x<=sizeX; ++x) {
                double sx = screenX(x*Tile::size);
                p.drawLine(sx, screenY(0), sx, screenY(m_object->height()));
            }

            // horizontal
            for(int y=0; y<=sizeY; ++y) {
                double sy = screenY(y*Tile::size);
                p.drawLine(screenX(0), sy, screenX(m_object->width()), sy);
            }

        } else if( gridType == EditorSettings::Fast ) {
            p.setPen(QColor(128, 128, 128));

            for(int y = 0; y <= sizeY; ++y) {
                for(int x = 0; x <= sizeX; ++x)
                    p.drawPoint((int)screenX(x*Tile::size), (int)screenY(y*Tile::size));
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

void ObjectView::on_btnLeftPlus_clicked()
{
    m_object->addTilesLeft(1);
    refreshProperties();
    update();
}

void ObjectView::on_btnLeftMinus_clicked()
{
    m_object->addTilesLeft(-1);
    refreshProperties();
    update();
}

void ObjectView::on_btnRightPlus_clicked()
{
    m_object->addTilesRight(1);
    refreshProperties();
    update();
}

void ObjectView::on_btnRightMinus_clicked()
{
    m_object->addTilesRight(-1);
    refreshProperties();
    update();
}

void ObjectView::on_btnTopPlus_clicked()
{
    m_object->addTilesTop(1);
    refreshProperties();
    update();
}

void ObjectView::on_btnTopMinus_clicked()
{
    m_object->addTilesTop(-1);
    refreshProperties();
    update();
}

void ObjectView::on_btnBottomPlus_clicked()
{
    m_object->addTilesBottom(1);
    refreshProperties();
    update();
}

void ObjectView::on_btnBottomMinus_clicked()
{
    m_object->addTilesBottom(-1);
    refreshProperties();
    update();
}

void ObjectView::verticalScroll(int value)
{
    m_offsetY = value;
    this->update();
}

void ObjectView::horizontalScroll(int value)
{
    m_offsetX = value;
    this->update();
}

void ObjectView::setUpScrolling()
{
    // set up scroll bars
    const int bufferRoom = 800;
    m_hsb->setMinimum((int)(-bufferRoom));
    m_hsb->setMaximum((int)(m_object->width()));
    m_hsb->setValue((int)0);
    m_vsb->setMinimum((int)(-bufferRoom));
    m_vsb->setMaximum((int)(m_object->height()));
    m_vsb->setValue((int)0);
}

void ObjectView::setZoom(double zoom)
{
    m_zoom = zoom;
    this->update();
}

void ObjectView::zoomIn()
{
    m_zoom = Utils::min(m_zoom * 1.2, 100.0);
    this->update();
}

void ObjectView::zoomOut()
{
    m_zoom = Utils::max(m_zoom * 0.8, 0.2);
    this->update();
}

void ObjectView::addLayer()
{
    Debug::assert(m_object, "Error: Layer manipulations on a null object");
    m_object->addLayer();

    refreshLayersList();
    this->update();
}

void ObjectView::deleteLayer(int index)
{
    Debug::assert(m_object, "Error: Layer manipulations on a null object");
    m_object->deleteLayer(index);

    refreshLayersList();
    this->update();
}

void ObjectView::swapLayers(int i, int j)
{
    Debug::assert(m_object, "Error: Layer manipulations on a null object");
    m_object->swapLayer(i, j);

    refreshLayersList();
    this->update();
}

void ObjectView::refreshProperties()
{
    QTableWidget * tbl = m_window->propertiesTable();
    tbl->item(Name, 1)->setText(m_objectName);
    tbl->item(Width, 1)->setText(QString::number((int) m_object->width() / Tile::size));
    tbl->item(Height, 1)->setText(QString::number((int) m_object->height() / Tile::size));
    tbl->item(Description, 1)->setText(QObject::tr("description not supported yet"));
}

void ObjectView::propertyChanged(int row)
{
    TableProperty prop = (TableProperty) row;

    QTableWidget * tbl = m_window->propertiesTable();
    QString value = tbl->item(row, 1)->text();
    switch(prop) {
        case Name:
            m_objectName = value;
            break;
        case Width:
            m_object->setWidth(value.toInt() * Tile::size);
            break;
        case Height:
            m_object->setHeight(value.toInt() * Tile::size);
            break;
        case Description:
            // TODO: handle this case
            break;
        default:
            Debug::assert(false, "unhandled property");
            break;
    }

    refreshGui();
}
