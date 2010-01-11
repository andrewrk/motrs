#include "World.h"

#include "Utils.h"

World::World(const char * buffer)
{
    const char * cursor = buffer;
    int count = Utils::readInt(&cursor);
    m_good = true;
    for (int i = 0; i < count; i++) {
        int x = Utils::readInt(&cursor);
        int y = Utils::readInt(&cursor);
        int z = Utils::readInt(&cursor);
        Map * map = ResourceManager::getMap(Utils::readString(&cursor));
        if (!map->isGood()) {
            m_good = false;
            break;
        }
        m_maps.push_back(WorldMap(x, y, z, map));
    }
}

bool World::isGood() {
    return m_good;
}

Map * World::getMap() {
    return m_maps[0].map;
}
