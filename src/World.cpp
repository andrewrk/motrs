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
        m_maps.push_back(WorldMap(x, y, z, map));
    }
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

Map * World::getMap() {
    return m_maps[0].map;
}

Universe::Location World::locationOf(double absoluteX, double absoluteY)
{
    Universe::Location loc;

    // the easy parts
    loc.world = this;
    loc.x = absoluteX;
    loc.y = absoluteY;
    loc.z = -1; // don't know

    // find the map
    for(unsigned int i=0; i<m_maps.size(); ++i){
        WorldMap * wmap = &m_maps[i];
        if( absoluteX >= wmap->x && absoluteX < wmap->x + wmap->map->width() &&
            absoluteY >= wmap->y && absoluteY < wmap->y + wmap->map->height() )
        {
            loc.map = wmap->map;
            loc.mapX = wmap->x;
            loc.mapY = wmap->y;
            return loc;
        }
    }

    // can't find a map
    loc.mapX = -1;
    loc.mapY = -1;
    loc.map = NULL;
    return loc;
}
