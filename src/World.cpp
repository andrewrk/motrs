#include "World.h"

#include "Utils.h"

World::World(ResourceFile* resourceFile, std::string resourceName)
{
    char * buffer = resourceFile->getResource(resourceName);
    if (buffer == NULL) {
        std::cerr << "unable to load World: " << resourceName << std::endl;
        m_good = false;
        return;
    }
    char * cursor = buffer;
    int count = Utils::readInt(&cursor);
    m_good = true;
    for (int i = 0; i < count; i++) {
        int x = Utils::readInt(&cursor);
        int y = Utils::readInt(&cursor);
        int z = Utils::readInt(&cursor);
        Map * map = new Map(resourceFile, Utils::readString(&cursor));
        if (!map->isGood()) {
            m_good = false;
            break;
        }
        m_maps.push_back(WorldMap(x, y, z, map));
    }
    delete[] buffer;
}

bool World::isGood() {
    return m_good;
}
