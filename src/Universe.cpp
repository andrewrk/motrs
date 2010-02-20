#include "Universe.h"

#include "ResourceManager.h"

#include "Utils.h"
#include "Map.h"
#include "World.h"
#include "Entity.h"
#include "ResourceManager.h"

Universe::Universe() :
    m_worlds(),
    m_startWorld(-1),
    m_player(NULL)
{
}

Universe::~Universe()
{
}

Universe * Universe::load(const char * buffer)
{
    Universe * out = new Universe();

    const char * cursor = buffer;
    int version = Utils::readInt(&cursor);
    int codeVersion = 2;
    if (version != codeVersion) {
        std::cerr << "Unsupported Universe version: " << version << std::endl;
        delete out;
        return NULL;
    }

    int worldCount = Utils::readInt(&cursor);
    for (int i = 0; i < worldCount; i++) {
        std::string worldId = Utils::readString(&cursor);
        World * world = ResourceManager::getWorld(worldId);
        if (world == NULL) {
            std::cerr << "Cannot continue loading Universe because a world failed to load." << std::endl;
            delete out;
            return NULL;
        }
        out->m_worlds.push_back(world);
    }

    out->m_player = ResourceManager::getEntity(Utils::readString(&cursor));
    if (out->m_player == NULL) {
        std::cerr << "Error loading Universe: error loading player entity" << std::endl;
        delete out;
        return NULL;
    }

    out->m_startWorld = Utils::readInt(&cursor);
    double startX = (double)Utils::readInt(&cursor);
    double startY = (double)Utils::readInt(&cursor);
    int startZ = Utils::readInt(&cursor);
    out->m_player->setCenter(startX, startY);
    out->m_player->setLayer(startZ);
    out->m_player->setOrientation(Entity::Center);

    return out;
}

int Universe::worldCount() {
    return m_worlds.size();
}

World * Universe::startWorld() {
    return m_worlds[m_startWorld];
}
