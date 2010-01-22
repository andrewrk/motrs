#include "Universe.h"

#include "ResourceManager.h"

#include "Utils.h"
#include "Map.h"
#include "World.h"
#include "Entity.h"
#include "ResourceManager.h"

Universe::Universe(const char * buffer) :
    m_worlds(),
    m_good(true),
    m_startWorld(-1),
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
    if (m_player == NULL || !m_player->isGood()) {
        m_good = false;
        return;
    }

    m_startWorld = Utils::readInt(&cursor);
    double startX = (double)Utils::readInt(&cursor);
    double startY = (double)Utils::readInt(&cursor);
    int startZ = Utils::readInt(&cursor);
    m_player->setPosition(startX, startY, startZ, Entity::Center);

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

World * Universe::startWorld() {
    return m_worlds[m_startWorld];
}
