#include "World.h"

#include "Utils.h"
#include "ResourceManager.h"

World * World::load(const char * buffer)
{
    World * out = new World();

    const char * cursor = buffer;
    int version = Utils::readInt(&cursor);
    if (version != 1) {
        std::cerr << "Unsupported World version: " << version << std::endl;
        delete out;
        return NULL;
    }
    int mapCount = Utils::readInt(&cursor);
    for (int i = 0; i < mapCount; i++) {
        int x = Utils::readInt(&cursor);
        int y = Utils::readInt(&cursor);
        int z = Utils::readInt(&cursor);
        std::string mapId = Utils::readString(&cursor);
        Map * map = ResourceManager::getMap(mapId);
        if (map == NULL) {
            std::cerr << "Cannot load world because loading its maps failed" << std::endl;
            delete out;
            return NULL;
        }
        map->setPosition(x, y, z);
        out->m_maps.push_back(map);
    }

    return out;
}

World::World() :
    m_maps()
{
    m_maps.clear();
}

World::~World()
{

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
