#include "ResourceManager.h"

#include "Universe.h"

#include "Debug.h"

ResourceFile * ResourceManager::resourceFile = NULL;
std::map<std::string, World*> ResourceManager::s_worlds;
std::map<std::string, Map*> ResourceManager::s_maps;

Universe * ResourceManager::loadUniverse(std::string resourceFilePath, std::string id) {
    resourceFile = new ResourceFile(resourceFilePath);
    Universe * universe = new Universe(resourceFile->getResource(id));
    delete resourceFile;
    resourceFile = NULL;
    return universe;
}

World * ResourceManager::getWorld(std::string id) {
    Debug::assert(resourceFile != NULL, "ResourceManager::getWorld: resourceFile == NULL");
    World * world = find(s_worlds, id);
    if (world == NULL) {
        const char * buffer = resourceFile->getResource(id);
        if (buffer == NULL) {
            std::cerr << "unable to load World: " << id << std::endl;
            return NULL;
        }
        world = new World(buffer);
        delete[] buffer;
    }
    return world;
}

Map * ResourceManager::getMap(std::string id) {
    Debug::assert(resourceFile != NULL, "ResourceManager::get" "Map" ": resourceFile == NULL");
    Map * map = find(s_maps, id);
    if (map == NULL) {
        const char * buffer = resourceFile->getResource(id);
        if (buffer == NULL) {
            std::cerr << "unable to load Map: " << id << std::endl;
            return NULL;
        }
        map = new Map(buffer);
        delete[] buffer;
    }
    return map;
}


