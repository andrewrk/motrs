#include "Tile.h"

#include "Utils.h"
#include "Gameplay.h"

#include "SDL.h"

const double Tile::size = 16.0;

std::map<std::string, int> Tile::s_propertyNames;

Tile::Tile(const char** cursor) :
    m_good(true),
    m_shape(tsSolidWall),
    m_surfaceType(stNormal),
    m_graphic(NULL)
{
    m_shape = (Shape)Utils::readInt(cursor);
    m_surfaceType = (SurfaceType)Utils::readInt(cursor);
    int propCount = Utils::readInt(cursor);
    for (int i = 0; i < propCount; i++) {
        std::string key = Utils::readString(cursor);
        std::string value = Utils::readString(cursor);
        switch (s_propertyNames[key]) {
        case PropertyGraphic:
            if (m_graphic != NULL) {
                std::cerr << "overwriting Tile property Graphic" << std::endl;
                m_good = false;
            }
            m_graphic = ResourceManager::getGraphic(value);
            break;
        default:

            break;
        }
    }
}



bool Tile::isGood() {
    return m_good;
}

void Tile::draw(double screenX, double screenY) {
    m_graphic->draw(Gameplay::instance()->screen(), (int)screenX, (int)screenY);
}
