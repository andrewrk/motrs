#include "EditorTile.h"

#include "WorldView.h"

EditorTile::EditorTile(Shape shape, SurfaceType surfaceType, EditorGraphic * graphic) :
    Tile(),
    m_graphic(graphic)
{
    this->m_shape = shape;
    this->m_surfaceType = surfaceType;
}

EditorTile::~EditorTile()
{

}

void EditorTile::draw(int screenX, int screenY) {
    m_graphic->draw(*WorldView::painter(), (int)screenX, (int)screenY);
}
