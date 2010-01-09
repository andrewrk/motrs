#include "Universe.h"

Universe::Universe(ResourceFile * resourceFile, std::string resourceName)
    : PropertiesResource(resourceFile->getResource(resourceName))
{
    
}

Universe::~Universe()
{

}

World * Universe::firstWorld() {
    return m_worlds[0];
}
