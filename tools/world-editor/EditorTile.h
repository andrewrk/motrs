#ifndef EDITORTILE_H
#define EDITORTILE_H

#include "Tile.h"

#include "EditorGraphic.h"

#include <QPixmap>
#include <QPainter>

class EditorTile : public Tile
{
public:
    EditorTile(Shape shape, SurfaceType surfaceType, EditorGraphic * graphic);
    ~EditorTile();

    inline EditorGraphic * graphic();
    void draw(int screenX, int screenY);

private:
    EditorGraphic * m_graphic;
};

inline EditorGraphic * EditorTile::graphic()
{
    return m_graphic;
}

#endif // EDITORTILE_H
