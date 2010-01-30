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

void World::calculateBoundaries()
{
    if ( m_maps.size() == 0)
    {
        m_width = 0;
        m_height = 0;
        m_top = 0;
        m_left = 0;
        return;
    }

    // Calculate width
    double left = m_maps[0].x;
    double top = m_maps[0].y;
    double right = left + m_maps[0].map->width();
    double bottom = top + m_maps[0].map->height();

    for( unsigned int i = 1; i < m_maps.size(); i++)
    {
        if( left > m_maps[i].x )
            left = m_maps[i].x;

        if( top > m_maps[i].y )
            top = m_maps[i].y;

        if( right < m_maps[i].x + m_maps[i].map->width() )
            right = m_maps[i].x + m_maps[i].map->width();

        if( bottom < m_maps[i].y + m_maps[i].map->height() )
            bottom = m_maps[i].y + m_maps[i].map->height();
    }
    m_left = left;
    m_top = top;
    m_height = bottom - top;
    m_width = right - left;
}
