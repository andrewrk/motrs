#include "EditorObject.h"

EditorObject::EditorObject() :
    m_layerNames(),
    m_surfaceTypes(new Array3<Tile::SurfaceType>(1,1,1)),
    m_shapes(new Array3<Tile::Shape>(1,1,1)),
    m_name(QObject::tr("New Object")),
    m_description("")
{
    m_surfaceTypes->clear();
    m_shapes->clear();
    m_layerNames << QObject::tr("Layer 1");
}

EditorObject::~EditorObject()
{
}

EditorObject * EditorObject::load(QString file)
{
    // TODO
}

void EditorObject::save(QString file)
{
    // TODO
}

Tile::Shape EditorObject::shape(int x, int y, int z)
{
    return m_shapes->get(x,y,z);
}

void EditorObject::setShape(int x, int y, int z, Tile::Shape shape)
{
    m_shapes->set(x,y,z,shape);
}

Tile::SurfaceType EditorObject::surfaceType(int x, int y, int z)
{
    return m_surfaceTypes->get(x,y,z);
}

void EditorObject::setSurfaceType(int x, int y, int z, Tile::SurfaceType surfaceType)
{
    m_surfaceTypes->set(x,y,z,surfaceType);
}

void EditorObject::addLayer(QString name)
{
    QString newLayerName = name.isEmpty() ? QObject::tr("Layer %1").arg(QString::number(layerCount()+1))
        : name;
    m_shapes->redim(m_shapes->sizeX(), m_shapes->sizeY(), m_shapes->sizeZ()+1, (Tile::Shape) 0);
    m_surfaceTypes->redim(m_shapes->sizeX(), m_shapes->sizeY(), m_shapes->sizeZ(), (Tile::SurfaceType) 0);
    m_layerNames << newLayerName;
}

void EditorObject::deleteLayer(int index)
{
    m_shapes->deleteRowZ(index);
    m_surfaceTypes->deleteRowZ(index);
    m_layerNames.removeAt(index);
}

void EditorObject::swapLayer(int i, int j)
{
    m_shapes->swapRowZ(i, j);
    m_surfaceTypes->swapRowZ(i, j);
    m_layerNames.swap(i, j);
}

void EditorObject::addTilesLeft(int amount)
{
    m_shapes->expandLeft(amount, (Tile::Shape) 0);
    m_surfaceTypes->expandLeft(amount, (Tile::SurfaceType) 0);
}

void EditorObject::addTilesRight(int amount)
{
    m_shapes->expandRight(amount, (Tile::Shape) 0);
    m_surfaceTypes->expandRight(amount, (Tile::SurfaceType) 0);
}

void EditorObject::addTilesTop(int amount)
{
    m_shapes->expandTop(amount, (Tile::Shape) 0);
    m_surfaceTypes->expandTop(amount, (Tile::SurfaceType) 0);
}

void EditorObject::addTilesBottom(int amount)
{
    m_shapes->expandBottom(amount, (Tile::Shape) 0);
    m_surfaceTypes->expandBottom(amount, (Tile::SurfaceType) 0);
}

int EditorObject::tileCountX()
{
    return m_shapes->sizeX();
}

int EditorObject::tileCountY()
{
    return m_shapes->sizeY();
}

int EditorObject::layerCount()
{
    return m_shapes->sizeZ();
}

void EditorObject::renameLayer(int index, QString newName)
{
    m_layerNames.replace(index, newName);
}

QString EditorObject::layerName(int index)
{
    return m_layerNames.at(index);
}
