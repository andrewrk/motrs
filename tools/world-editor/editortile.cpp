#include "editortile.h"

EditorTile::EditorTile(Shape shape, SurfaceType surfaceType, QPixmap * pixmap) :
    Tile(),
    m_pixmap(pixmap)
{
    this->m_shape = shape;
    this->m_surfaceType = surfaceType;
}

EditorTile::~EditorTile()
{

}
