#ifndef EDITORMAP_H
#define EDITORMAP_H

#include "Map.h"
#include "EditorObject.h"
#include "EditorEntity.h"

#include <QString>
#include <QList>

// An EditorMap has a list of EditorObjects and EditorEntities that are tile-aligned.
// The game editor can use this class to edit maps that have movable EditorObjects and EditorEntities
// Call the compile() method to turn it into a real Map which will break
// the EditorObjects into tiles
class EditorMap : public Map
{
public: //variables
    typedef struct {
        EditorObject * object;
        int tileX; // tile index
        int tileY;
        int layer;
    } MapObject;

    typedef struct {
        // layer name
        QString name;
        // list of objects per layer. Layer 0 of the object == the layer it is on.
        // this means the object's layerCount + index < this editormap's layerCount
        QList<MapObject *> objects;
        // list of entities per layer
        QList<EditorEntity *> entities;
    } MapLayer;

public: //methods
    // default Map. empty dimensions and layers.
    EditorMap();
    ~EditorMap();

    // load a Map from a text format file
    static EditorMap * load(QString file);

    void setLeft(double value);
    void setTop(double value);
    void setWidth(double value);
    void setHeight(double value);

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

    // expose the layers structure
    inline MapLayer * layer(int index);

    double width();
    double height();
private: //variables
    // we simply remember the dimensions because we don't actually have a tile
    // grid until we compile into a Map
    int m_tileCountX;
    int m_tileCountY;

    QList<MapLayer *> m_layers;
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

inline EditorMap::MapLayer * EditorMap::layer(int index)
{
    return m_layers.at(index);
}

#endif
