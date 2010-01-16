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
    Universe * universe = new Universe(buffer + sizeof(char));
    delete resourceFile;
    resourceFile = NULL;
    if (!universe->isGood()) {
        delete universe;
        return NULL;
    }
    return universe;
}

World * ResourceManager::getWorld(std::string id) {
    return getResource("World", s_worlds, 'W', id);
}

Map * ResourceManager::getMap(std::string id) {
    return getResource("Map", s_maps, 'M', id);
}

Graphic * ResourceManager::getGraphic(std::string id) {
    return getResource("Graphic", s_graphics, 'G', id);
}

Character * ResourceManager::getCharacter(std::string id) {
    return getResource("Character", s_characters, 'C', id);
}

