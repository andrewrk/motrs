#include "EditorMap.h"

#include "EditorResourceManager.h"
#include "EditorEntity.h"

#include <QPixmap>
#include <QDir>
#include <QDebug>
#include <QPainter>

EditorMap::EditorMap() :
    m_tileCountX(0),
    m_tileCountY(0)
{
}

EditorMap * EditorMap::load(QString file)
{
    QList< QPair<QString, QString> > props;

    bool good = EditorResourceManager::loadTextFile(file, props);

    assert(good);
    if (! good ) {
        qDebug() << "Error reading EditorMap file";
        return NULL;
    }

    EditorMap * out = new EditorMap();

    int layerCount;

    for(int i=0; i<props.size(); ++i) {
        if( props[i].first.compare("version", Qt::CaseInsensitive) == 0 ) {
            int fileVersion = props[i].second.toInt();
            int codeVersion = 4;
            assert(fileVersion == codeVersion);
        } else if( props[i].first.compare("size", Qt::CaseInsensitive) == 0 ) {
            // size in number of tiles
            // size=sizeX,sizeY,layer
            QStringList size = props[i].second.split(",");
            out->m_tileCountX = size.at(0).toInt();
            out->m_tileCountY = size.at(1).toInt();
            layerCount = size.at(2).toInt();
        } else if( props[i].first.compare("layer", Qt::CaseInsensitive) == 0 ) {
            // layer=name
            QStringList layerProps = props[i].second.split(",");
            out->addLayer(layerProps.at(0));
        } else if( props[i].first.compare("entity", Qt::CaseInsensitive) == 0 ) {
            // entity=x,y,layer,id
            QStringList entityProps = props[i].second.split(",");
            double x = entityProps.at(0).toDouble();
            double y = entityProps.at(1).toDouble();
            int layerIndex = entityProps.at(2).toInt();
            QString entityFileTitle = entityProps.at(3);

            QString entityFile = QDir(EditorResourceManager::entitiesDir()).absoluteFilePath(entityFileTitle);
            EditorEntity * entity = EditorEntity::load(entityFile);

            assert(entity);
            if (entity == NULL) {
                qDebug() << "Unable to load entity for EditorMap";
                delete out;
                return NULL; // TODO: fix this memory leak
            }

            entity->setCenter(x, y);
            entity->setLayer(layerIndex);
            out->m_layers.at(layerIndex)->entities.append(entity);
        } else if( props[i].first.compare("object", Qt::CaseInsensitive) == 0 ) {
            // object=tileX,tileY,layerIndex,objectId
            QStringList objectProps = props[i].second.split(",");
            MapObject * object = new MapObject;

            object->tileX = objectProps.at(0).toInt();
            object->tileY = objectProps.at(1).toInt();
            object->layer = objectProps.at(2).toInt();
            object->object = EditorObject::load(QDir(EditorResourceManager::objectsDir()).absoluteFilePath(objectProps.at(3)));

            assert(object->object);
            if (! object->object) {
                qDebug() << "Unable to load object for EditorMap";
                delete out;
                return NULL; // TODO: fix memory leak
            }

            out->m_layers.at(object->layer)->objects.append(object);
        } else {
            // unrecognized map property
            assert(false);
        }
    }

    assert(layerCount == out->m_layers.size());

    return out;
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
        m_tileCountX = tileCount;
}

void EditorMap::setHeight(double value)
{
    int tileCount = (int)(value / Tile::size);
    if (tileCount > 0)
        m_tileCountY = value;
}

void EditorMap::addLayer(QString name)
{
    MapLayer * layer = new MapLayer;
    layer->name = name.isEmpty() ?
        QObject::tr("Layer %1").arg(QString::number(layerCount())) : name;
    m_layers.append(layer);
}

void EditorMap::deleteLayer(int index)
{
    delete m_layers.takeAt(index);
}

void EditorMap::swapLayer(int i, int j)
{
    m_layers.swap(i, j);
}

void EditorMap::renameLayer(int index, QString newName)
{
    m_layers.at(index)->name = newName;
}

QString EditorMap::layerName(int index)
{
    return m_layers.at(index)->name;
}

void EditorMap::addTilesLeft(int amount)
{
    m_x -= amount * Tile::size;
    m_tileCountX += amount;
}

void EditorMap::addTilesRight(int amount)
{
    m_tileCountX += amount;
}

void EditorMap::addTilesTop(int amount)
{
    m_y -= amount * Tile::size;
    m_tileCountY += amount;
}

void EditorMap::addTilesBottom(int amount)
{
    m_tileCountY += amount;
}

double EditorMap::width()
{
    return m_tileCountX * Tile::size;
}

double EditorMap::height()
{
    return m_tileCountY * Tile::size;
}
