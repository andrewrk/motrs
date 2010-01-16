#include "World.h"

#include "Utils.h"

World::World(const char * buffer) :
    m_good(true)
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
        m_maps.push_back(WorldMap(x, y, z, map));
    }
}

bool World::isGood() {
    return m_good;
}

Map * World::getMap() {
    return m_maps[0].map;
}
