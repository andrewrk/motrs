#ifndef EDITORMAP_H
#define EDITORMAP_H

#include "EditorObject.h"
#include "ResourceFile.h"

#include <QString>
#include <QList>

class EditorWorld;
class EditorEntity;

// An EditorMap has a list of EditorObjects that are tile-aligned and
// EditorEntities that are pixel-aligned (but still with respect to the map)
// The game editor can use this class to edit maps that have movable EditorObjects and EditorEntities
class EditorMap
{
public: //variables
    struct MapObject {
        EditorObject * object;
        int tileX; // tile index
        int tileY;
        int layer;

        // the map that owns this object
        EditorMap * parent;

        QRectF geometry()
        {
            return QRectF(QPointF(tileX*Tile::size, tileY*Tile::size),
                QSizeF(object->tileCountX()*Tile::size, object->tileCountY()*Tile::size));
        }

        MapObject() :
            object(NULL),
            parent(NULL)
        {
        }
    };

    struct MapLayer {
        // layer name
        QString name;
        // list of objects per layer. Layer 0 of the object == the layer it is on.
        // this means the object's layerCount + index < this editormap's layerCount
        // if an object has multiple layers, there is a pointer to the same object
        // in each layer list in which the object's layer overlaps
        QList<MapObject *> objects;
        // list of entities per layer
        QList<EditorEntity *> entities;
    };

public: //methods
    // default Map. empty dimensions and layers.
    ~EditorMap();

    // load a Map from a text format file
    static EditorMap * load(QString file);

    // create an empty map
    static EditorMap * createEmpty(const QString & name, const QRect & geometry);

    void save();

    void setLeft(int value);
    void setTop(int value);
    void setWidth(int value);
    void setHeight(int value);
    void setPosition(int x, int y, int story);

    inline int left();
    inline int top();
    int width();
    int height();
    QRectF geometry();

    void addLayer(QString name = "");
    void deleteLayer(int index);
    void swapLayer(int i, int j);
    void renameLayer(int index, QString newName);
    QString layerName(int index);
    inline int layerCount();

    // add / remove(negative amount) tiles
    void addTilesLeft(int amount);
    void addTilesRight(int amount);
    void addTilesTop(int amount);
    void addTilesBottom(int amount);

    // return the number of tiles
    inline int tileCountX();
    inline int tileCountY();

    // expose the layers structure, read only.
    inline const MapLayer * layer(int index) const;

    // add/remove an object/entity to this map. makes sure the data structure is intact.
    void addObject(MapObject * object);
    void removeObject(MapObject * object);

    void addEntity(EditorEntity * entity);
    void removeEntity(EditorEntity * entity);

    inline QString name() const;

    // set the parent world of the map
    void setWorld(EditorWorld * world);
    EditorWorld * world();

    // compile the map and its dependencies, and put into resources file.
    // returns success
    bool build(ResourceFile & resources);

private: //variables
    static const int c_codeVersion;

    struct PaletteEntry {
        int shape;
        int surfaceType;
        QString graphicId;
    };

    // we simply remember the dimensions because we don't actually have a tile
    // grid until we compile into a Map
    int m_tileCountX;
    int m_tileCountY;

    QList<MapLayer *> m_layers;

    QString m_name;

    EditorWorld * m_world;

    int m_left;
    int m_top;
    int m_story;
private: //methods
    EditorMap();
};

inline int EditorMap::tileCountX()
{
    return m_tileCountX;
}

inline int EditorMap::tileCountY()
{
    return m_tileCountY;
}

inline int EditorMap::layerCount()
{
    return m_layers.size();
}

inline const EditorMap::MapLayer * EditorMap::layer(int index) const
{
    return m_layers.at(index);
}

inline QString EditorMap::name() const
{
    return m_name;
}

inline int EditorMap::left()
{
    return m_left;
}

inline int EditorMap::top()
{
    return m_top;
}

#endif
