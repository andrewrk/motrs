#include "Tile.h"

const int Tile::size = 16;

Tile::Tile(ResourceFile * resourceFile, std::string resourceName)
    : PropertiesResource(resourceFile, resourceName)
{

}
