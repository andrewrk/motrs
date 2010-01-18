#ifndef _RESORCE_MANAGER_H_
#define _RESORCE_MANAGER_H_

#include "ResourceFile.h"

#include "Debug.h"

class Universe;
class World;
class Map;
class Graphic;
class Entity;

class ResourceManager {
public:
    static Universe * loadUniverse(std::string resourceFilePath, std::string id);

    static World * getWorld(std::string id);
    static Map * getMap(std::string id);
    static Graphic * getGraphic(std::string id);
    static Entity * getEntity(std::string id);

private:
    static ResourceFile * resourceFile;

    static std::map<std::string, World*> s_worlds;
    static std::map<std::string, Map*> s_maps;
    static std::map<std::string, Graphic*> s_graphics;
    static std::map<std::string, Entity*> s_entities;

    template <class T>
    static T * find(std::map<std::string, T*> & map, std::string id) {
        typename std::map<std::string, T*>::const_iterator it = map.find(id);
        if (it == map.end())
            return NULL;
        return it->second;
    }

    template <class T>
    static T * getResource(std::string resourceTypeName, std::map<std::string, T*> & cache, char typeCode, std::string id) {
        Debug::assert(resourceFile != NULL, "ResourceManager::get" + resourceTypeName + ": resourceFile == NULL");
        T * resource = find(cache, id);
        if (resource == NULL) {
            const char * buffer = resourceFile->getResource(id);
            if (buffer == NULL) {
                std::cerr << "Unable to find " + resourceTypeName + ": " << id << std::endl;
                return NULL;
            }
            char actualTypeCode = *buffer;
            if (actualTypeCode != typeCode) {
                std::cerr << "Wrong type code in resource " << id << ". " <<
                        "Should be '" << typeCode << "' but it's '" << actualTypeCode << "'." << std::endl;
                delete buffer;
                return NULL;
            }
            resource = new T(buffer + sizeof(char));
            delete[] buffer;
        }
        return resource;
    }
};

#endif
