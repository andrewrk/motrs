#ifndef EDITORMAP_H
#define EDITORMAP_H

#include "Map.h"

#include <QString>
#include <QStringList>

// An EditorMap has a list of EditorObjects that are tile-aligned.
// The game editor can use this class to edit maps that have movable EditorObjects
// Call the compile() method to turn it into a real Map which will break
// the EditorObjects into tiles
class EditorMap : public Map
{
public:
    // load a Map from a text format file
    EditorMap(QString file);
    // default Map. arbitrary defaults.
    EditorMap();
    virtual ~EditorMap();

    void draw(double screenX, double screenY, double screenWidth,
              double screenHeight, int layer);

    void setLeft(double value);
    void setTop(double value);
    void setWidth(double value);
    void setHeight(double value);

    void addLayer(QString name = "");
    void deleteLayer(int index);
    void swapLayer(int i, int j);
    void renameLayer(int index, QString newName);
    QString layerName(int index);

    // add / remove(negative amount) tiles
    void addTilesLeft(int amount);
    void addTilesRight(int amount);
    void addTilesTop(int amount);
    void addTilesBottom(int amount);

    Tile * tile(int x, int y, int z);

    // it's about time we have these convenience methods...
    int tileCountX();
    int tileCountY();
private:
    QStringList m_layerNames;


};

#endif
