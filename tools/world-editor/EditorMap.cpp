#include "EditorMap.h"

#include "EditorResourceManager.h"
#include "EditorEntity.h"
#include "EditorWorld.h"

#include <QPixmap>
#include <QDir>
#include <QDebug>
#include <QPainter>

EditorMap::EditorMap() :
    m_tileCountX(0),
    m_tileCountY(0),
    m_world(NULL)
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

    out->m_name = QFileInfo(file).fileName();

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
            out->addEntity(entity);
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

            out->addObject(object);
        } else {
            // unrecognized map property
            assert(false);
        }
    }

    assert(layerCount == out->layerCount());

    return out;
}

EditorMap::~EditorMap()
{
}

void EditorMap::save()
{
    QString filename = QDir(EditorResourceManager::mapsDir()).absoluteFilePath(m_name);
    QFile file(filename);
    file.open(QIODevice::WriteOnly | QIODevice::Text);

    if( ! file.isOpen() ){
        qDebug() << "Unable to open " << filename << " for writing";
        return;
    }

    QTextStream out(&file);

    out << "version=4\n";
    out << "# size=sizeX,sizeY,layerCount\n";
    out << "# sizes in number of tiles, not in actual coordinates.\n";
    out << "size=" << tileCountX() << "," << tileCountY() << "," << layerCount() << "\n\n";

    out << "# layer declarations. one declaration per layer\n";
    out << "# layer=layerName\n";
    for (int i=0; i<layerCount(); ++i)
        out << "layer=" << layerName(i) << "\n";
    out << "\n";

    out << "# object declarations. uses tile coordinates\n";
    out << "# object=tileX,tileY,layerIndex,objectId\n";
    for (int layerIndex=0; layerIndex<m_layers.size(); ++layerIndex) {
        QList<MapObject *> objects = m_layers.at(layerIndex)->objects;
        for (int i=0; i<objects.size(); ++i) {
            MapObject * object = objects.at(i);
            if (object->layer == layerIndex)
                out << "object=" << object->tileX << "," << object->tileY << "," << layerIndex << "," << object->object->resourceName() << "\n";
        }
    }
    out << "\n";

    out << "# entity declarations. use absolute coordinates\n";
    out << "# entity=x,y,layerIndex,entityId\n";
    for (int layerIndex=0; layerIndex<m_layers.size(); ++layerIndex) {
        QList<EditorEntity *> entities = m_layers.at(layerIndex)->entities;
        for (int i=0; i<entities.size(); ++i) {
            EditorEntity * entity = entities.at(i);
            out << "entity=" << entity->centerX() << "," << entity->centerY() << "," << entity->layer() << "," << entity->name() << "\n";
        }
    }
}

void EditorMap::addObject(MapObject * object)
{
    for (int i=0; i<object->object->layerCount(); ++i) {
        int layerIndex = object->layer + i;
        while (layerIndex >= layerCount())
            addLayer();
        m_layers.at(layerIndex)->objects.append(object);
    }
}

void EditorMap::removeObject(MapObject * object)
{
    for (int i=0; i<object->object->layerCount(); ++i) {
        int layerIndex = object->layer + i;
        m_layers.at(layerIndex)->objects.removeOne(object);
    }
}

void EditorMap::addEntity(EditorEntity * entity)
{
    while (entity->layer() >= layerCount())
        addLayer();
    m_layers.at(entity->layer())->entities.append(entity);
}

void EditorMap::removeEntity(EditorEntity * entity)
{
    m_layers.at(entity->layer())->entities.removeOne(entity);
}

void EditorMap::setLeft(double value)
{
    this->m_x = value;

    if (m_world != NULL)
        m_world->calculateBoundaries();
}

void EditorMap::setTop(double value)
{
    this->m_y = value;

    if (m_world != NULL)
        m_world->calculateBoundaries();
}

void EditorMap::setWidth(double value)
{
    int tileCount = (int)(value / Tile::size);
    if (tileCount > 0)
        m_tileCountX = tileCount;

    if (m_world != NULL)
        m_world->calculateBoundaries();
}

void EditorMap::setHeight(double value)
{
    int tileCount = (int)(value / Tile::size);
    if (tileCount > 0)
        m_tileCountY = tileCount;

    if (m_world != NULL)
        m_world->calculateBoundaries();
}

void EditorMap::addLayer(QString name)
{
    MapLayer * layer = new MapLayer;
    layer->name = name.isEmpty() ?
        QObject::tr("Layer %1").arg(QString::number(layerCount()+1)) : name;
    m_layers.append(layer);
}

void EditorMap::deleteLayer(int index)
{
    MapLayer * layer = m_layers.at(index);
    for (int i=0; i<layer->entities.size(); ++i)
        removeEntity(layer->entities.at(i));

    // remove all objects associated with this layer, saving the ones that
    // we want to put back later
    QList<MapObject *> savedObjects;
    while (layer->objects.size() > 0) {
        MapObject * object = layer->objects.first();
        if (object->layer != index)
            savedObjects.append(object);
        removeObject(object);
    }

    // delete the layer
    delete layer;
    m_layers.removeAt(index);

    // put the ones back in that we saved
    for (int i=0; i<savedObjects.size(); ++i)
        addObject(savedObjects.at(i));
}

void EditorMap::swapLayer(int i, int j)
{
    // take out the objects
    QList<MapObject *> savedObjects;
    for (int layer=i; layer<=j; ++layer) {
        QList<MapObject *> objects = m_layers.at(layer)->objects;
        while (objects.size() > 0) {
            MapObject * object = objects.first();
            savedObjects.append(object);
            removeObject(object);
        }
    }

    // swap the layer
    m_layers.swap(i, j);

    // put the objects back in
    for (int i=0; i<savedObjects.size(); ++i)
        addObject(savedObjects.at(i));
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

    if (m_world != NULL)
        m_world->calculateBoundaries();
}

void EditorMap::addTilesRight(int amount)
{
    m_tileCountX += amount;

    if (m_world != NULL)
        m_world->calculateBoundaries();
}

void EditorMap::addTilesTop(int amount)
{
    m_y -= amount * Tile::size;
    m_tileCountY += amount;

    if (m_world != NULL)
        m_world->calculateBoundaries();
}

void EditorMap::addTilesBottom(int amount)
{
    m_tileCountY += amount;

    if (m_world != NULL)
        m_world->calculateBoundaries();
}

double EditorMap::width()
{
    return m_tileCountX * Tile::size;
}

double EditorMap::height()
{
    return m_tileCountY * Tile::size;
}

QRectF EditorMap::geometry()
{
    return QRectF(QPointF(left(), top()), QSizeF(width(), height()));
}

void EditorMap::setWorld(EditorWorld * world)
{
    m_world = world;
}

EditorWorld * EditorMap::world()
{
    return m_world;
}

EditorMap * EditorMap::createEmpty(const QString & name, const QRect & geometry)
{
    EditorMap * out = new EditorMap();
    out->m_name = name;

    out->setLeft(geometry.left());
    out->setTop(geometry.top());
    out->setWidth(geometry.width());
    out->setHeight(geometry.height());

    out->addLayer(QObject::tr("Layer 1"));

    return out;
}
