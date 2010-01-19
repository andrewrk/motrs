#ifndef EDITORTILE_H
#define EDITORTILE_H

#include "Tile.h"

#include <QPixmap>

class EditorTile : public Tile
{
public:
    EditorTile(Shape shape, SurfaceType surfaceType, QPixmap * pixmap);
    ~EditorTile();

    inline QPixmap * pixmap() { return m_pixmap; }
private:
    QPixmap * m_pixmap;
};

#endif // EDITORTILE_H
