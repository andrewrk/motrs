#include "ResourceManager.h"

#include "Universe.h"
#include "Entity.h"

#include "Debug.h"

ResourceFile * ResourceManager::resourceFile = NULL;

std::map<std::string, const char *> ResourceManager::s_worlds;
std::map<std::string, const char *> ResourceManager::s_maps;
std::map<std::string, const char *> ResourceManager::s_entities;

std::map<std::string, Graphic*> ResourceManager::s_graphics;

Universe * ResourceManager::loadUniverse(std::string resourceFilePath, std::string id) {
    resourceFile = new ResourceFile(resourceFilePath);
    const char * buffer = resourceFile->getResource(id);
    if (buffer == NULL) {
        std::cerr << "Unable to find Universe: " << id << std::endl;
        delete resourceFile;
        return NULL;
    }
    char actualTypeCode = *buffer;
    if (actualTypeCode != 'U') {
        std::cerr << "Wrong type code in resource " << id << ". " <<
                "Should be 'U' but it's '" << actualTypeCode << "'." << std::endl;
        delete resourceFile;
        delete buffer;
        return NULL;
    }
    Universe * universe = Universe::load(buffer + sizeof(char));
    delete resourceFile;
    resourceFile = NULL;
    if (universe == NULL) {
        std::cerr << "Unable to return universe - it did not load correctly." << std::endl;
        return NULL;
    }
    // TODO: clean caches of buffers
    return universe;
}

World * ResourceManager::getWorld(std::string id) {
    return loadFromCachedBuffer<World>("World", s_worlds, 'W', id);
}

Map * ResourceManager::getMap(std::string id) {
    return loadFromCachedBuffer<Map>("Map", s_maps, 'M', id);
}

Entity * ResourceManager::getEntity(std::string id) {
    return loadFromCachedBuffer<Entity>("Entity", s_entities, 'E', id);
}

Graphic * ResourceManager::getGraphic(std::string id) {
    return loadCachedResource<Graphic>("Graphic", s_graphics, 'G', id);
}

