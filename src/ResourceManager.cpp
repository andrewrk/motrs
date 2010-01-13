#include "ResourceManager.h"

#include "Universe.h"
#include "Character.h"

#include "Debug.h"

ResourceFile * ResourceManager::resourceFile = NULL;

std::map<std::string, World*> ResourceManager::s_worlds;
std::map<std::string, Map*> ResourceManager::s_maps;
std::map<std::string, Graphic*> ResourceManager::s_graphics;
std::map<std::string, Character*> ResourceManager::s_characters;

Universe * ResourceManager::loadUniverse(std::string resourceFilePath, std::string id) {
    resourceFile = new ResourceFile(resourceFilePath);
    const char * buffer = resourceFile->getResource(id);
    if (buffer == NULL) {
        std::cerr << "unable to load Universe: " << id << std::endl;
        delete resourceFile;
        return NULL;
    }
    Universe * universe = new Universe(buffer);
    delete resourceFile;
    resourceFile = NULL;
    Debug::assert(universe->isGood(), "ResourceManager::loadUniverse - "
        "error loading universe");
    return universe;
}

//RESOURCE_GETTER(World, world, "World")
//RESOURCE_GETTER(Map, map, "Map")
//RESOURCE_GETTER(Graphic, graphic, "Graphic")
//RESOURCE_GETTER(Character, character, "Character")

World * ResourceManager::getWorld(std::string id) {
    return getResource("World", s_worlds, id);
}

Map * ResourceManager::getMap(std::string id) {
    return getResource("Map", s_maps, id);
}

Graphic * ResourceManager::getGraphic(std::string id) {
    return getResource("Graphic", s_graphics, id);
}

Character * ResourceManager::getCharacter(std::string id) {
    return getResource("Character", s_characters, id);
}

