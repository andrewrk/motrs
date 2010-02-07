#include "EditorMap.h"

#include "EditorResourceManager.h"
#include "EditorTile.h"
#include "EditorEntity.h"

#include <QPixmap>
#include <QDir>
#include <QDebug>
#include <QPainter>

EditorMap::EditorMap(QString file)
{
    QVector< QPair<QString, QString> > props;

    bool good = EditorResourceManager::loadTextFile(file, props);

    if( ! good )
        return;

    int sizeX, sizeY, layerCount;
    int layerIndex = 0;

    // implicit null tile
    m_palette.clear();
    m_palette.push_back(Tile::nullTile());
    m_layerNames.clear();
    m_entities.clear();

    for(int i=0; i<props.size(); ++i) {
        if( props[i].first.compare("version", Qt::CaseInsensitive) == 0 ) {
            int fileVersion = props[i].second.toInt();
            int codeVersion = 3;
            if( fileVersion != codeVersion ) {
                qDebug() << "Tried to open version  " << fileVersion <<
                    " with version " << codeVersion << " code.";
                good = false;
                return;
            }
        } else if( props[i].first.compare("size", Qt::CaseInsensitive) == 0 ) {
            // size in number of tiles
            // size=sizeX,sizeY
            QStringList size = props[i].second.split(",");
            sizeX = size.at(0).toInt();
            sizeY = size.at(1).toInt();
            layerCount = size.at(2).toInt();

            m_tiles = new Array3<int>(sizeX, sizeY, layerCount);
        } else if( props[i].first.compare("tile", Qt::CaseInsensitive) == 0 ) {
            // tile=shape,surface,graphicId
            QStringList tileProps = props[i].second.split(",");
            QString graphicFile = tileProps.at(2);
            QPixmap * pixmap = EditorResourceManager::pixmapForGraphic(graphicFile);
            EditorTile * tile = new EditorTile(
                (EditorTile::Shape)tileProps.at(0).toInt(),
                (EditorTile::SurfaceType)tileProps.at(1).toInt(),
                pixmap);
            m_palette.push_back(tile);
        } else if( props[i].first.compare("layer", Qt::CaseInsensitive) == 0 ) {
            // layer=name,0,0,...
            QStringList layer = props[i].second.split(",");
            m_layerNames << layer.takeFirst();
            int index = 0;
            for(int y = 0; y < sizeY; ++y) {
                for(int x = 0; x < sizeX; ++x) {
                    m_tiles->set(x,y,layerIndex,layer[index].toInt());
                    ++index;
                }
            }
            ++layerIndex;
        } else if( props[i].first.compare("entity", Qt::CaseInsensitive) == 0 ) {
            // entity=x,y,layer,id
            QStringList entityProps = props[i].second.split(",");
            double x = entityProps.at(0).toDouble();
            double y = entityProps.at(1).toDouble();
            int layerIndex = entityProps.at(2).toInt();
            QString entityFileTitle = entityProps.at(3);

            QDir dir(EditorResourceManager::dataDir());
            dir.cd("entities");
            QString entityFile = dir.absoluteFilePath(entityFileTitle);
            EditorEntity * entity = new EditorEntity(entityFile);
            entity->setCenter(x, y);
            entity->setLayer(layerIndex);
            m_entities.push_back(entity);
        } else {
            qDebug() << "Unrecognized Map property: " << props[i].first
                << " = " << props[i].second;
            good = false;
            return;
        }
    }

    calculateBoundaries();
}

void EditorMap::draw(double screenX, double screenY,
                     double screenWidth, double screenHeight, int layer)
{
    Map::draw(screenX, screenY, screenWidth, screenHeight, layer);
}

EditorMap::~EditorMap()
{
}

void EditorMap::setLeft(double value)
{
    this->m_x = value;
}

void EditorMap::setTop(double value)
{
    this->m_y = value;
}

void EditorMap::setWidth(double value)
{
    int tileCount = (int)(value / Tile::size);
    if (tileCount > 0)
        m_tiles->redim(tileCount, m_tiles->sizeY(), m_tiles->sizeZ(), 0);
    calculateBoundaries();
}

void EditorMap::setHeight(double value)
{
    int tileCount = (int)(value / Tile::size);
    if (tileCount > 0)
        m_tiles->redim(m_tiles->sizeX(), tileCount, m_tiles->sizeZ(), 0);
    calculateBoundaries();
}


void EditorMap::addLayer(QString name)
{
    QString newLayerName = name.isEmpty() ? QObject::tr("Layer %1").arg(QString::number(layerCount()))
        : name;
    m_tiles->redim(m_tiles->sizeX(), m_tiles->sizeY(), m_tiles->sizeZ()+1, 0);
    m_layerNames << newLayerName;
}

void EditorMap::deleteLayer(int index)
{
    m_tiles->deleteRowZ(index);
    m_layerNames.removeAt(index);
}

void EditorMap::swapLayer(int i, int j)
{
    m_tiles->swapRowZ(i, j);
    m_layerNames.swap(i, j);
}

void EditorMap::renameLayer(int index, QString newName)
{
    m_layerNames.replace(index, newName);
}

QString EditorMap::layerName(int index)
{
    return m_layerNames.at(index);
}

