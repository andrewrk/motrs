#include "PropertiesResource.h"

#include <iostream>


std::map<std::string, int> PropertiesResource::s_types;

PropertiesResource::PropertiesResource(const char * buffer) :
    m_properties(),
    m_error(false)
{
    // initialize map between type strings and enum values
    if( s_types.size() == 0 ) {
        s_types.insert(
            std::pair<std::string, int>("string", Variant::TypeString) );
        s_types.insert(
            std::pair<std::string, int>("color", Variant::TypeRGB) );
        s_types.insert(
            std::pair<std::string, int>("int", Variant::TypeInt) );
        s_types.insert(
            std::pair<std::string, int>("float", Variant::TypeFloat) );
        s_types.insert(
            std::pair<std::string, int>("bool", Variant::TypeBool) );
    }

    // read properties from buffer
    Header * header = (Header*) buffer;

    if( header->version != 1 ) {
        std::cerr << "PropertiesResource: found a file with version "
            << header->version << ", can only read up to version 1.\n";
        m_error = true;
        return;
    }

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

PropertiesResource::Variant PropertiesResource::readValue(int type, const char ** ptr)
{
    // return a value of type type and advance the read pointer
    Variant val;
    switch(type) {
        case Variant::TypeInt:
            val.setValue( *((int *) (*ptr)) );
        case Variant::TypeFloat:
            val.setValue( *((float *) (*ptr)) );
        case Variant::TypeBool:
            val.setValue( *((bool *) (*ptr)) );
        case Variant::TypeString:
            val.setValue( readString(ptr) );
        case Variant::TypeRGB:
            val.setValue( *((Variant::RGB *) (*ptr)) );
        default:
            std::cerr << "PropertiesResource: unrecognized type\n";
            m_error = true;
    }
    return val;
}

// return a string and advance the read pointer
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
    return ! m_error;
}

// get the value of a property
PropertiesResource::Variant PropertiesResource::property(std::string propName)
{
    return m_properties[propName];
}
