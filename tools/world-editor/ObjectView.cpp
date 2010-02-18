#include "ObjectView.h"

#include "EditorResourceManager.h"
#include "ObjectEditor.h"
#include "EditorSettings.h"
#include "Utils.h"
#include "EditorGlobals.h"

#include <QPainter>
#include <QStandardItemModel>
#include <QFileInfo>
#include <QDebug>
#include <QMessageBox>
#include <QTableWidget>
#include <QDir>
#include <QAction>
#include <QDockWidget>

#include <cmath>

#include "moc_ObjectView.cxx"

QVector<QPixmap *> ObjectView::s_surfaceTypePixmaps;
QStringList ObjectView::s_surfaceTypeNames;

QVector<QPixmap *> ObjectView::s_shapePixmaps;
QStringList ObjectView::s_shapeNames;

const char * ObjectView::c_shapes[] = {
    "tsSolidWall",
    "tsSolidFloor",
    "tsSolidHole",
    "tsDiagFloorWallNW",
    "tsDiagFloorWallNE",
    "tsDiagFloorWallSE",
    "tsDiagFloorWallSW",
    "tsFloorRailN",
    "tsFloorRailE",
    "tsFloorRailS",
    "tsFloorRailW"
};

const char * ObjectView::c_surfaceTypes[] = {
    "stNormal",
    "stWater",
    "stIce",
    "stQuickSand"
};

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
    m_viewMode(vmNormal),
    m_zoom(1.0),
    m_offsetX(0),
    m_offsetY(0),
    m_selectedGraphic(NULL),
    m_mouseState(msNormal)
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

    initializePixmapCache();

    this->setMouseTracking(true);
    this->setAcceptDrops(true);
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
    m_object = new EditorObject();
    setViewMode(vmNormal);
    refreshGui();
}


void ObjectView::refreshSurfaceTypes()
{
    QListWidget * list = m_window->surfaceTypesList();

    if( list->count() == 0 ) {
        list->clear();
        for(int i=0; i<s_surfaceTypePixmaps.size(); ++i) {
            QListWidgetItem * item = new QListWidgetItem(QIcon(*s_surfaceTypePixmaps.at(i)), s_surfaceTypeNames[i], list);
            list->addItem(item);
        }

        if( list->count() > 0 )
            list->setCurrentRow(0);
    }
}

void ObjectView::refreshShapes()
{
    QListWidget * list = m_window->shapesList();

    if( list->count() == 0 ) {
        list->clear();
        for(int i=0; i<s_shapePixmaps.size(); ++i) {
            QListWidgetItem * item = new QListWidgetItem(QIcon(*s_shapePixmaps.at(i)), s_shapeNames[i], list);
            list->addItem(item);
        }

        if( list->count() > 0 )
            list->setCurrentRow(0);
    }
}

void ObjectView::refreshArt()
{
    QListWidget * artList = m_window->artList();

    // do a directory listing of data/art
    QDir dir(EditorResourceManager::dataDir());
    dir.cd("art");
    QStringList filters;
    filters << "*.bmp" << "*.png" << "*.jpg";
    QStringList entries = dir.entryList(filters, QDir::Files | QDir::Readable,
        QDir::Name | QDir::IgnoreCase);
    artList->clear();
    for(int i=0; i<entries.size(); ++i) {
        // create item
        QString file = dir.absoluteFilePath(entries[i]);
        QPixmap * pixmap = new QPixmap(file);
        QListWidgetItem * item = new QListWidgetItem(QIcon(*pixmap), entries[i], artList);
        item->setData(Qt::UserRole, QVariant(file));
        artList->addItem(item);
    }
}

void ObjectView::refreshGui()
{
    refreshArt();
    refreshSurfaceTypes();
    refreshShapes();
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
        if( m_viewMode == vmNormal ) {
            QHash<int, QList<EditorObject::ObjectGraphic *> *> * graphics = m_object->graphics();
            for(int layer=0; layer<m_object->layerCount(); ++layer) {
                if( m_window->layersList()->item(layer)->checkState() == Qt::Checked ) {
                    // draw all art items at this layer
                    QList<EditorObject::ObjectGraphic *> * thisLayerGraphics = graphics->value(layer);
                    for(int i=0; i<thisLayerGraphics->size(); ++i) {
                        EditorObject::ObjectGraphic * graphic = thisLayerGraphics->at(i);
                        p.drawPixmap(screenX(graphic->x), screenY(graphic->y),
                            graphic->width * m_zoom, graphic->height * m_zoom,
                            *graphic->pixmap);
                    }

                    // draw dragged stuff
                    if( layer == m_selectedLayer && m_dragPixmap != NULL ) {
                        p.drawPixmap(m_dragPixmapX, m_dragPixmapY,
                            m_dragPixmap->width() * m_zoom,
                            m_dragPixmap->height() * m_zoom, *m_dragPixmap);
                    }
                }
            }

            // draw selection rectangle around selected graphic
            if( m_selectedGraphic != NULL ) {
                p.setPen(QPen(Qt::blue,2));

                QRect outline(screenX(m_selectedGraphic->x), screenY(m_selectedGraphic->y),
                    m_selectedGraphic->width * m_zoom,
                    m_selectedGraphic->height * m_zoom);

                p.drawRect(outline);
            }
        } else if( m_viewMode == vmSurfaceType || m_viewMode == vmShape ) {
            int sizeX = m_object->tileCountX();
            int sizeY = m_object->tileCountY();
            for (int x=0; x<sizeX; ++x) {
                for(int y=0; y<sizeY; ++y) {
                    QPixmap * pixmap = (m_viewMode == vmSurfaceType) ?
                        s_surfaceTypePixmaps[m_object->surfaceType(x,y,m_selectedLayer)] :
                        s_shapePixmaps[m_object->shape(x,y,m_selectedLayer)];
                    p.drawPixmap(screenX(x * Tile::size), screenY(y * Tile::size), Tile::size * m_zoom, Tile::size * m_zoom, *pixmap);
                }
            }
        } else {
            Debug::assert(false, "Unknown view mode");
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
            // it's art. create a new ObjectGraphic and add it to the object
            EditorObject::ObjectGraphic * graphic = new EditorObject::ObjectGraphic;
            graphic->pixmapFile = info.fileName();
            graphic->pixmap = EditorResourceManager::pixmapForArt(graphic->pixmapFile);
            graphic->x = absoluteX(e->pos().x());
            graphic->y = absoluteY(e->pos().y());
            graphic->width = graphic->pixmap->width();
            graphic->height = graphic->pixmap->height();
            graphic->layer = m_selectedLayer;

            m_object->graphics()->value(m_selectedLayer)->append(graphic);
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
    this->update();
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
    this->update();
    setControlEnableStates();
}

void ObjectView::open(QString file)
{
    if( m_object )
        delete m_object;
    m_object = EditorObject::load(file);

    setViewMode(vmNormal);
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
            list->setCurrentRow(0);
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
    m_window->viewModeNormalAction()->setChecked(false);
    m_window->viewModeShapeAction()->setChecked(false);
    m_window->viewModeSurfaceTypeAction()->setChecked(false);
    m_window->artDock()->hide();
    m_window->shapesDock()->hide();
    m_window->surfaceTypesDock()->hide();
    switch(mode) {
        case vmNormal:
            m_window->viewModeNormalAction()->setChecked(true);
            m_window->artDock()->show();
            break;
        case vmShape:
            m_window->viewModeShapeAction()->setChecked(true);
            m_window->shapesDock()->show();
            break;
        case vmSurfaceType:
            m_window->viewModeSurfaceTypeAction()->setChecked(true);
            m_window->surfaceTypesDock()->show();
            break;
    }
    update();
}

void ObjectView::drawGrid(QPainter &p)
{
    EditorSettings::GridRenderType gridType = EditorSettings::gridRenderType();
    if( gridType != EditorSettings::None ) {
        int sizeX = m_object->tileCountX();
        int sizeY = m_object->tileCountY();

        if( gridType == EditorSettings::Pretty ) {
            p.setPen(QColor(128, 128, 128, 64));

            // vertical
            for(int x=0; x<=sizeX; ++x) {
                double sx = screenX(x*Tile::size);
                p.drawLine(sx, screenY(0), sx, screenY(sizeY * Tile::size));
            }

            // horizontal
            for(int y=0; y<=sizeY; ++y) {
                double sy = screenY(y*Tile::size);
                p.drawLine(screenX(0), sy, screenX(sizeX * Tile::size), sy);
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
    m_hsb->setMaximum((int)(m_object->tileCountX()*Tile::size));
    m_hsb->setValue((int)0);
    m_vsb->setMinimum((int)(-bufferRoom));
    m_vsb->setMaximum((int)(m_object->tileCountY()*Tile::size));
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
    tbl->item(Name, 1)->setText(m_object->name());
    tbl->item(Width, 1)->setText(QString::number(m_object->tileCountX()));
    tbl->item(Height, 1)->setText(QString::number(m_object->tileCountY()));
    tbl->item(Description, 1)->setText(m_object->description());
}

void ObjectView::propertyChanged(int row)
{
    TableProperty prop = (TableProperty) row;

    QTableWidget * tbl = m_window->propertiesTable();
    QString value = tbl->item(row, 1)->text();
    switch(prop) {
        case Name:
            m_object->setName(value);
            break;
        case Width:
            m_object->addTilesRight(value.toInt() - m_object->tileCountX());
            break;
        case Height:
            m_object->addTilesBottom(value.toInt() - m_object->tileCountY());
            break;
        case Description:
            m_object->setDescription(value);
            break;
        default:
            Debug::assert(false, "unhandled property");
            break;
    }

    refreshGui();
}

void ObjectView::initializePixmapCache()
{
    if( s_shapePixmaps.size() == 0 ) {
        QDir dir(EditorResourceManager::localDataDir());
        for(int i=0; i<Tile::tsCount; ++i) {
            QString name(c_shapes[i]);
            s_shapeNames << name;
            name.append(".png");
            s_shapePixmaps.append(new QPixmap(dir.absoluteFilePath(name)));
        }
    }

    if( s_surfaceTypePixmaps.size() == 0 ) {
        QDir dir(EditorResourceManager::localDataDir());
        for(int i=0; i<Tile::stCount; ++i) {
            QString name(c_surfaceTypes[i]);
            s_surfaceTypeNames << name;
            name.append(".png");
            s_surfaceTypePixmaps.append(new QPixmap(dir.absoluteFilePath(name)));
        }
    }
}

EditorObject::ObjectGraphic * ObjectView::graphicAt(int x, int y)
{
    double absX = absoluteX(x);
    double absY = absoluteY(y);
    QHash<int, QList<EditorObject::ObjectGraphic *> *> * graphics = m_object->graphics();
    for(int layer=m_object->layerCount()-1; layer >= 0; --layer) {
        if( m_window->layersList()->item(layer)->checkState() == Qt::Checked ) {
            QList<EditorObject::ObjectGraphic *> * list = graphics->value(layer);
            for(int i=0; i<list->size(); ++i) {
                EditorObject::ObjectGraphic * graphic = list->at(i);
                if( absX >= graphic->x && absX <= graphic->x + graphic->width &&
                    absY >= graphic->y && absY <= graphic->y + graphic->height )
                {
                    return graphic;
                }
            }
        }
    }
    return NULL;
}

bool ObjectView::overGraphicLeft(int x, int y)
{
    if( ! m_selectedGraphic )
        return false;

    double absX = absoluteX(x);
    double absY = absoluteY(y);

    return  absX > m_selectedGraphic->x - g_lineSelectRadius &&
            absX < m_selectedGraphic->x + g_lineSelectRadius &&
            absY > m_selectedGraphic->y && absY < m_selectedGraphic->y + m_selectedGraphic->height;
}

bool ObjectView::overGraphicTop(int x, int y)
{
    if( ! m_selectedGraphic )
        return false;

    double absX = absoluteX(x);
    double absY = absoluteY(y);

    return  absX > m_selectedGraphic->x && absX < m_selectedGraphic->x + m_selectedGraphic->width &&
            absY > m_selectedGraphic->y - g_lineSelectRadius &&
            absY < m_selectedGraphic->y + g_lineSelectRadius;
}
bool ObjectView::overGraphicRight(int x, int y)
{
    if( ! m_selectedGraphic )
        return false;

    double absX = absoluteX(x);
    double absY = absoluteY(y);

    return  absX > m_selectedGraphic->x + m_selectedGraphic->width - g_lineSelectRadius &&
            absX < m_selectedGraphic->x + m_selectedGraphic->width + g_lineSelectRadius &&
            absY > m_selectedGraphic->y && absY < m_selectedGraphic->y + m_selectedGraphic->height;

}
bool ObjectView::overGraphicBottom(int x, int y)
{
    if( ! m_selectedGraphic )
        return false;

    double absX = absoluteX(x);
    double absY = absoluteY(y);

    return  absX > m_selectedGraphic->x && absX < m_selectedGraphic->x + m_selectedGraphic->width &&
            absY > m_selectedGraphic->y + m_selectedGraphic->height - g_lineSelectRadius &&
            absY < m_selectedGraphic->y + m_selectedGraphic->height + g_lineSelectRadius;
}

bool ObjectView::overSelectedGraphic(int x, int y)
{
    if( ! m_selectedGraphic )
        return false;

    double absX = absoluteX(x);
    double absY = absoluteY(y);

    return  absX > m_selectedGraphic->x && absX < m_selectedGraphic->x + m_selectedGraphic->width &&
            absY > m_selectedGraphic->y && absY < m_selectedGraphic->y + m_selectedGraphic->height;
}

void ObjectView::mousePressEvent(QMouseEvent * e)
{
    m_mouseDownX = e->x();
    m_mouseDownY = e->y();

    if( m_viewMode == vmSurfaceType || m_viewMode == vmShape ) {
        mouseMoveEvent(e);
    } else if( m_viewMode == vmNormal ) {
        if( m_mouseState == msNormal ) {
            // are we stretching the boundaries of a graphic?
            if( m_selectedGraphic ) {
                m_startX = m_selectedGraphic->x;
                m_startY = m_selectedGraphic->y;
                m_startWidth = m_selectedGraphic->width;
                m_startHeight = m_selectedGraphic->height;
            }

            if( overGraphicLeft(e->x(), e->y()) )
                m_mouseState = msStretchGraphicLeft;
            else if( overGraphicRight(e->x(), e->y()) )
                m_mouseState = msStretchGraphicRight;
            else if( overGraphicTop(e->x(), e->y()) )
                m_mouseState = msStretchGraphicTop;
            else if( overGraphicBottom(e->x(), e->y()) )
                m_mouseState = msStretchGraphicBottom;
            else if( overSelectedGraphic(e->x(), e->y()) )
                m_mouseState = msMoveGraphic;
            else
                m_selectedGraphic = graphicAt(e->x(), e->y()); // select a graphic if they click it

            this->update();
            determineCursor();
        }
    } else {
        Debug::assert(false, "Invalid viewMode");
    }
}

void ObjectView::mouseReleaseEvent(QMouseEvent * e)
{
    doDragAction(e->x(), e->y());

    // return state to normal
    m_mouseState = msNormal;
    determineCursor();
}

void ObjectView::doDragAction(int x, int y)
{
    double deltaX = (x - m_mouseDownX) * m_zoom;
    double deltaY = (y - m_mouseDownY) * m_zoom;
    switch(m_mouseState) {
        case msStretchGraphicLeft:
            m_selectedGraphic->x = m_startX + deltaX;
            m_selectedGraphic->width = m_startWidth + deltaX;
        break;
        case msStretchGraphicTop:
            m_selectedGraphic->y = m_startY + deltaY;
            m_selectedGraphic->height = m_startHeight + deltaY;
        break;
        case msStretchGraphicRight:
            m_selectedGraphic->width = m_startWidth + deltaX;
        break;
        case msStretchGraphicBottom:
            m_selectedGraphic->height = m_startHeight + deltaY;
        break;
        case msMoveGraphic:
            m_selectedGraphic->x = m_startX + deltaX;
            m_selectedGraphic->y = m_startY + deltaY;
        break;
        case msNormal: // do nothing
        break;
    }
}

void ObjectView::determineCursor()
{
    switch(m_viewMode) {
        case vmNormal:
            // change mouse cursor to sizers if over graphic boundaries
            if( overGraphicLeft(m_mouseX, m_mouseY) || overGraphicRight(m_mouseX, m_mouseY))
                this->setCursor(Qt::SizeHorCursor);
            else if( overGraphicTop(m_mouseX, m_mouseY) || overGraphicBottom(m_mouseX, m_mouseY))
                this->setCursor(Qt::SizeVerCursor);
            else if( overSelectedGraphic(m_mouseX, m_mouseY))
                this->setCursor(Qt::SizeAllCursor);
            else
                this->setCursor(Qt::ArrowCursor);
            break;
        case vmShape:
        case vmSurfaceType:
            this->setCursor(Qt::ArrowCursor);
        default:
            Debug::assert(false, "invalid viewMode");
    }
}

void ObjectView::mouseMoveEvent(QMouseEvent * e)
{
    m_mouseX = e->x();
    m_mouseY = e->y();

    determineCursor();

    if( m_viewMode == vmNormal ) {
        if( m_mouseState != msNormal ) {
            doDragAction(e->x(), e->y());
            this->update();
        }
    } else if( m_viewMode == vmSurfaceType || m_viewMode == vmShape ) {
        int tileX = absoluteX(e->x()) / Tile::size;
        int tileY = absoluteY(e->y()) / Tile::size;
        int tileZ = m_selectedLayer;
        bool inRange =  tileX >= 0 && tileX < m_object->tileCountX() &&
                        tileY >= 0 && tileY < m_object->tileCountY() &&
                        tileZ >= 0;

        // are they over a tile and clicking the mouse?
        if( inRange && (e->buttons() & Qt::LeftButton) ) {
            // paint the tile they are over with the selected surface type or shape
            if( m_viewMode == vmSurfaceType ) {
                m_object->setSurfaceType(tileX, tileY, tileZ,
                    (Tile::SurfaceType)m_window->surfaceTypesList()->currentRow());
            } else {
                m_object->setShape(tileX, tileY, tileZ,
                    (Tile::Shape)m_window->shapesList()->currentRow());
            }
            this->update();
        }
    } else {
        Debug::assert(false, "Invalid viewMode");
    }
}
