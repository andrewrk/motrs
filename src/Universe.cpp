#include "Universe.h"

#include "Utils.h"

Universe::Universe(const char * buffer) :
    m_worlds(),
    m_good(true),
    m_startWorld(-1),
    m_startX(-1),
    m_startY(-1),
    m_startZ(-1),
    m_player(NULL)
{
    const char * cursor = buffer;
    int version = Utils::readInt(&cursor);
    if (version != 2) {
        std::cerr << "Unsupported Universe version: " << version << std::endl;
        m_good = false;
        return;
    }

    int worldCount = Utils::readInt(&cursor);
    for (int i = 0; i < worldCount; i++) {
        std::string worldId = Utils::readString(&cursor);
        World * world = ResourceManager::getWorld(worldId);
        if (world == NULL) {
            m_good = false;
            return;
        }
        m_worlds.push_back(world);
    }

    m_player = ResourceManager::getEntity(Utils::readString(&cursor));
    if (m_player == NULL) {
        m_good = false;
        return;
    }

    m_startWorld = Utils::readInt(&cursor);
    m_startX = Utils::readInt(&cursor);
    m_startY = Utils::readInt(&cursor);
    m_startZ = Utils::readInt(&cursor);
}

Universe::~Universe()
{
}

bool Universe::isGood() {
    return m_good;
}

int Universe::worldCount() {
    return m_worlds.size();
}

World * Universe::firstWorld() {
    return m_worlds[0];
}
