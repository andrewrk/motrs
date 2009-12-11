#ifndef RESOURCE_H
#define RESOURCE_H

#include <string>
#include <iostream>
#include <fstream>
#include <vector>

// load and save resources from a file
class ResourceFile
{
    public:
        ResourceFile();
        ResourceFile(std::string fileName);
        ~ResourceFile();

        // return whether or not the resource file is working
        bool isOpen();

        // attempt to open a resource file
        void open(std::string fileName);

        // create a new resources file
        void createNew(std::string fileName);

        // deallocate memory - you can let the destructor handle this
        // if you want
        void close();

        // copy the resource into memory and return a pointer
        // it's your job to deallocate the resource.
        char * getResource(std::string resourceName);

        // add a resource to the file
        void addResource(std::string resourceName, char * data,
            unsigned long dataSize);

        // delete a resource from a file
        bool deleteResource(std::string resourceName);

    private:
        static const unsigned long int initialMaxResources;

        enum States {
            StateUninitialized,
            StateReady,
            StateError
        };

        typedef struct {
            char name[128]; // 128 bytes
            unsigned long int offset; // 4 bytes, where the resource is
            unsigned long int size; // 4 bytes
        } ResourceRecord;

        typedef struct {
            unsigned long dataStart; // 4 bytes
            unsigned long resourceCount; // 4 bytes
        } ResourceHeader;

        int m_state;
        std::string m_fileName;
        std::fstream m_file;

        ResourceHeader m_header;

        unsigned long int recordLocation(unsigned long int resourceIndex);
        void loadRecordTable(std::vector<ResourceRecord> & v);
        void saveRecordTable(std::vector<ResourceRecord> & v);
        unsigned long int fileSize();
        static bool recordSortPredicate(const ResourceRecord &r1,
            const ResourceRecord &r2);
};

#endif
