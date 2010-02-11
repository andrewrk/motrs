#ifndef EDITORMAP_H
#define EDITORMAP_H

#include "Map.h"

#include <QString>
#include <QStringList>

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

    // save in text format
    void save(QString file);

    // add/remove (negative) tiles
    void addTilesLeft(int amount);
    void addTilesRight(int amount);
    void addTilesTop(int amount);
    void addTilesBottom(int amount);
private:
    QStringList m_layerNames;

};

#endif
