#include "World.h"

#include "Utils.h"

World::World(ResourceFile* resourceFile, std::string resourceName)
    : m_good(false)
{
    const char * buffer = resourceFile->getResource(resourceName);
    int* count = (int*)buffer;
    const char * cursor = buffer + sizeof(int);
    m_storyCount = 0;
    for (int i = 0; i < *count; i++) {
        int x = Utils::readInt(&cursor);
        int y = Utils::readInt(&cursor);
        int z = Utils::readInt(&cursor);
        Map * map = new Map(resourceFile, Utils::readString(&cursor));
        if (!map->isGood())
            return;
        m_maps.push_back(WorldMap(x, y, z, map));
        m_storyCount = Utils::max(m_storyCount, z);
    }
    m_good = true;
}

bool World::isGood() {
    return m_good;
}
