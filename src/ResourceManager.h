#ifndef _RESORCE_MANAGER_H_
#define _RESORCE_MANAGER_H_

#include "ResourceFile.h"

class Universe;
class World;
class Map;

class ResourceManager {
public:
    static Universe * loadUniverse(std::string resourceFilePath, std::string id);

    static World * getWorld(std::string id);
    static Map * getMap(std::string id);
private:
    static ResourceFile * resourceFile;

    static std::map<std::string, World*> s_worlds;
    static std::map<std::string, Map*> s_maps;

    template <class T>
    static T* find(std::map<std::string, T*> & map, std::string id) {
        typename std::map<std::string, T*>::const_iterator it = map.find(id);
        if (it == map.end())
            return NULL;
        return it->second;
    }
};

#endif
