#ifndef EDITORMAP_H
#define EDITORMAP_H

#include "Map.h"

#include <QString>
#include <QPainter>

class EditorMap : public Map
{
public:
    // load a Map from a text format file
    EditorMap(QString file);
    ~EditorMap();

    void draw(QPainter * p, double screenX, double screenY, double screenWidth,
              double screenHeight, int layer);

    static QPainter * m_painter;

};

#endif // EDITORMAP_H
