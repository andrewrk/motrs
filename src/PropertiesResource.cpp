#include "PropertiesResource.h"

#include "Utils.h"

#include <iostream>


std::map<std::string, int> PropertiesResource::s_types;

PropertiesResource::PropertiesResource(ResourceFile * resourceFile, std::string resourceName) :
    m_properties(),
    m_good(true)
{
    ensureTypesLoaded();
    const char * buffer = resourceFile->getResource(resourceName);
    if (buffer == NULL) {
        std::cerr << "unable to load Properties Resouce: " << resourceName << std::endl;
        m_good = false;
        return;
    }

    // read properties from buffer
    Header * header = (Header*) buffer;

    if (header->version != 1) {
        std::cerr << "PropertiesResource: found a file with version "
            << header->version << ", can only read up to version 1.\n";
        m_good = false;
    }
    if (m_good) {
        const char * ptr = buffer + sizeof(Header);
        for(unsigned int i=0; i<header->propertyCount; ++i) {
            // read the property
            std::string name = readString(&ptr);
            std::string type = readString(&ptr);
            Variant val = readValue(s_types[type], &ptr);

            // add to map
            m_properties.insert(std::pair<std::string, Variant>(name, val));
        }
    }
    delete[] buffer;
}

void PropertiesResource::ensureTypesLoaded() {
    if( s_types.size() == 0 ) {
        s_types["string"] = Variant::TypeString;
        s_types["color"] = Variant::TypeRGB;
        s_types["int"] = Variant::TypeInt;
        s_types["float"] = Variant::TypeFloat;
        s_types["bool"] = Variant::TypeBool;
    }
}

PropertiesResource::Variant PropertiesResource::readValue(int type, const char ** ptr)
{
    // return a value of type type and advance the read pointer
    Variant val;
    switch(type) {
        case Variant::TypeInt:
            val.setValue(Utils::readInt(ptr));
            break;
        case Variant::TypeFloat:
            val.setValue(*Utils::readStruct<float>(ptr));
            break;
        case Variant::TypeBool:
            val.setValue(*Utils::readStruct<bool>(ptr));
            break;
        case Variant::TypeString:
            val.setValue(readString(ptr));
            break;
        case Variant::TypeRGB:
            val.setValue(*Utils::readStruct<Variant::RGB>(ptr));
            break;
        default:
            std::cerr << "PropertiesResource: unrecognized type: " << type << std::endl;
            m_good = false;
            break;
    }
    return val;
}

// return a string and advance the read pointer
// TODO replace Util::readString with this function
std::string PropertiesResource::readString(const char ** readPtr)
{
    int * size = (int*)(*readPtr);

    *readPtr += sizeof(int);
    std::string out(*readPtr, *size);

    *readPtr += out.size();

    return out;
}

PropertiesResource::~PropertiesResource()
{

}

bool PropertiesResource::isGood()
{
    return m_good;
}

// get the value of a property
PropertiesResource::Variant PropertiesResource::property(std::string propName)
{
    return m_properties[propName];
}
