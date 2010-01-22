#ifndef EDITORTILE_H
#define EDITORTILE_H

#include "Tile.h"

#include <QPixmap>
#include <QPainter>

class EditorTile : public Tile
{
public:
    EditorTile(Shape shape, SurfaceType surfaceType, QPixmap * pixmap);
    ~EditorTile();

    inline QPixmap * pixmap() { return m_pixmap; }
    void draw(double screenX, double screenY);

private:
    QPixmap * m_pixmap;
};

#endif // EDITORTILE_H
