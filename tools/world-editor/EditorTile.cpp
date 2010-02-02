#include "EditorTile.h"

#include "WorldView.h"

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

void EditorTile::draw(int screenX, int screenY) {
    WorldView::painter()->drawPixmap((int)screenX, (int)screenY, *m_pixmap );
}
