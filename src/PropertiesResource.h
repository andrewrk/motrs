#ifndef _PROPERTIES_RESOURCE_H_
#define _PROPERTIES_RESOURCE_H_

#include "ResourceManager.h"

#include <map>
#include <string>

class PropertiesResource
{
public:
    class Variant
    {
    public:
        typedef struct {
            unsigned char r,g,b;
        } RGB;

        typedef union {
            int intValue;
            float floatValue;
            RGB rgbValue;
            bool boolValue;
        } TypesUnion;

        enum TypeNames {
            TypeInt,
            TypeFloat,
            TypeBool,
            TypeString,
            TypeRGB
        };

        Variant(){}
        Variant(int value){ setValue(value); }
        Variant(float value){ setValue(value); }
        Variant(RGB value){ setValue(value); }
        Variant(bool value){ setValue(value); }
        Variant(std::string value){ setValue(value); }

        void setValue(int value){ otherValue.intValue = value; }
        void setValue(float value){ otherValue.floatValue = value; }
        void setValue(RGB value){ otherValue.rgbValue = value; }
        void setValue(bool value){ otherValue.boolValue = value; }
        void setValue(std::string value){ stringValue = value; }

        ~Variant(){}

        std::string stringValue;
        TypesUnion otherValue;
    };

    // load object from memory
    PropertiesResource(const char * buffer);
    ~PropertiesResource();

    // tell whether this object is in a successful state or not
    bool isGood();

    // get the value of a property
    Variant property(std::string propName);

private:
    // map between type strings and enum values
    static std::map<std::string, int> s_types;
    static void ensureTypesLoaded();

    typedef struct {
        unsigned int version; // Uint32, file version
        unsigned int propertyCount; // Uint32, number of properties
    } Header;

    std::map<std::string, Variant> m_properties;

    bool m_good; // always true if things are good

    Variant readValue(int type, const char ** ptr);

};

#endif

