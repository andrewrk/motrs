#include "World.h"

#include "ResourceManager.h"

#include "Utils.h"
#include "ResourceManager.h"

World * World::load(const char * buffer) {
    return new World(buffer);
}

World::World(const char * buffer) :
    m_good(true),
    m_maps()
{
    const char * cursor = buffer;
    int version = Utils::readInt(&cursor);
    if (version != 1) {
        std::cerr << "Unsupported World version: " << version << std::endl;
        m_good = false;
        return;
    }
    int mapCount = Utils::readInt(&cursor);
    for (int i = 0; i < mapCount; i++) {
        int x = Utils::readInt(&cursor);
        int y = Utils::readInt(&cursor);
        int z = Utils::readInt(&cursor);
        std::string mapId = Utils::readString(&cursor);
        Map * map = ResourceManager::getMap(mapId);
        if (!map->isGood()) {
            m_good = false;
            break;
        }
        map->setPosition(x, y, z);
        m_maps.push_back(map);
    }

    calculateBoundaries();
}

World::World() :
    m_good(true),
    m_maps()
{
    m_maps.clear();
}

World::~World()
{

}

bool World::isGood() {
    return m_good;
}

Universe::Location World::locationOf(double absoluteX, double absoluteY) {
    Universe::Location loc;

    // the easy parts
    loc.world = this;
    loc.x = absoluteX;
    loc.y = absoluteY;
    loc.z = -1; // don't know

    // find the map
    for (unsigned int i = 0; i < m_maps.size(); i++) {
        Map * map = m_maps[i];
        if (absoluteX >= map->left() && absoluteX < map->left() + map->width() &&
            absoluteY >= map->top() && absoluteY < map->top() + map->height())
        {
            loc.map = map;
            loc.mapX = map->left();
            loc.mapY = map->top();
            return loc;
        }
    }

    // can't find a map
    loc.mapX = -1;
    loc.mapY = -1;
    loc.map = NULL;
    return loc;
}

void World::calculateBoundaries() {
    if (m_maps.size() == 0) {
        m_width = 0;
        m_height = 0;
        m_top = 0;
        m_left = 0;
        return;
    }

    // Calculate width
    double left = m_maps[0]->left();
    double top = m_maps[0]->top();
    double right = left + m_maps[0]->width();
    double bottom = top + m_maps[0]->height();

    for (unsigned int i = 1; i < m_maps.size(); i++) {
        if (left > m_maps[i]->left())
            left = m_maps[i]->left();

        if (top > m_maps[i]->top())
            top = m_maps[i]->top();

        if (right < m_maps[i]->left() + m_maps[i]->width())
            right = m_maps[i]->left() + m_maps[i]->width();

        if (bottom < m_maps[i]->top() + m_maps[i]->height())
            bottom = m_maps[i]->top() + m_maps[i]->height();
    }
    m_left = left;
    m_top = top;
    m_height = bottom - top;
    m_width = right - left;
}
