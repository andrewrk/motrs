#include "EditorTile.h"

#include "EditorMap.h"

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

void EditorTile::draw(double screenX, double screenY) {
    EditorMap::m_painter->drawPixmap((int)screenX, (int)screenY, *pixmap() );
}
