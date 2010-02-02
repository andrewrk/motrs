#ifndef EDITORMAP_H
#define EDITORMAP_H

#include "Map.h"

#include <QString>
#include <QPainter>
#include <QStringList>

class EditorMap : public Map
{
public:
    // load a Map from a text format file
    EditorMap(QString file);
    virtual ~EditorMap();

    void draw(QPainter * p, double screenX, double screenY, double screenWidth,
              double screenHeight, int layer);

    static QPainter * m_painter;

    void setLeft(double value);
    void setTop(double value);
    void setWidth(double value);
    void setHeight(double value);

    void addLayer();
    void deleteLayer(int index);
    void swapLayer(int i, int j);
    void renameLayer(int index, QString newName);
    QString layerName(int index);
private:
    QStringList m_layerNames;

};

#endif
