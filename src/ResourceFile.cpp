#include "ResourceFile.h"

#include <algorithm>
#include <cstring>

const unsigned long int ResourceFile::initialMaxResources = 100;

ResourceFile::ResourceFile() :
    m_state(StateUninitialized),
    m_fileName(),
    m_file()
{

}

ResourceFile::ResourceFile(std::string fileName) :
    m_state(StateUninitialized),
    m_fileName(),
    m_file()
{
    open(fileName);
}

ResourceFile::~ResourceFile()
{
    close();
}

bool ResourceFile::isOpen()
{
    return m_state == StateReady;
}

void ResourceFile::open(std::string fileName)
{
    close();
    m_fileName = fileName;

    m_file.open(m_fileName.c_str(), std::fstream::in | std::fstream::out);
    if( ! m_file.is_open() ) {
        m_state = StateError;
        return;
    }

    // header
    m_file.read((char*)&m_header, sizeof(m_header));
}

void ResourceFile::createNew(std::string fileName)
{
    close();
    m_fileName = fileName;
    
    m_file.open(m_fileName.c_str(), std::fstream::in | std::fstream::out);
    
    if( ! m_file.is_open() ){
        m_state = StateError;
        return;
    }
    
    // write the initial resource table
    // leave room for extra resource names
    ResourceHeader header;
    header.dataStart = recordLocation(initialMaxResources+1);
    header.resourceCount = 0;
    m_file.write((char *)&header, sizeof(header));
    
    // write all that padding until data start
    unsigned long int paddingSize = header.dataStart - sizeof(header);
    char * padding = new char[paddingSize];
    m_file.write(padding, paddingSize);
    delete[] padding;
}

void ResourceFile::close()
{
    if( m_state == StateReady ) {
        m_file.close();
        m_state = StateUninitialized;
    }
}

char * ResourceFile::getResource(std::string resourceName)
{
    // use the binary search algorithm to find the location
    int left = 0;
    int middle;
    int right = recordLocation(m_header.resourceCount);
    ResourceRecord record;
    int cmp;

    while(true) {
        middle = recordLocation(m_header.resourceCount / 2);
        
        // get the string at this location
        m_file.seekg(recordLocation(middle), std::ios_base::beg);
        m_file.read((char*)&record, sizeof(record));
        
        cmp = resourceName.compare(record.name);

        if( cmp > 0 ) {
            if( left == middle )
                left = right;
            else if( left == right )
                return NULL;
            else
                left = middle;
        } else if( cmp < 0 ) {
            if( right == middle )
                right = left;
            else if( left == right )
                return NULL;
            else
                right = middle;
        } else {
            // found it!
            break;
        }
    }

    m_file.seekg(record.offset, std::ios_base::beg);
    char * buffer = new char[record.size];
    m_file.read(buffer, record.size);

    return buffer;
}

unsigned long int ResourceFile::recordLocation(unsigned long int resourceIndex)
{
    return sizeof(ResourceHeader) + resourceIndex * sizeof(ResourceRecord);
}

bool ResourceFile::recordSortPredicate(const ResourceRecord &r1,
    const ResourceRecord &r2)
{
    return std::strcmp(r1.name, r2.name) < 0;
}

void ResourceFile::addResource(std::string resourceName, char * data,
    unsigned long int dataSize)
{
    // load the string table into a vector
    std::vector<ResourceRecord> table;
    loadRecordTable(table);

    // add the new resource record
    m_file.seekp(0, std::ios::end);

    ResourceRecord record;
    std::strcpy(record.name, resourceName.c_str());
    record.size = dataSize;
    record.offset = m_file.tellp();

    table.push_back(record);

    // write the resource to the end of the file
    m_file.write(data, dataSize);
    
    // sort the table
    std::sort(table.begin(), table.end(), recordSortPredicate);

    // write the record table back into the file
    saveRecordTable(table);

}

bool ResourceFile::deleteResource(std::string resourceName)
{
    // load the string table into a vector
    std::vector<ResourceRecord> table;
    loadRecordTable(table);

    // delete the element
    bool found = false;
    for( std::vector<ResourceRecord>::iterator it=table.begin();
        it < table.end(); ++it)
    {
        if( resourceName.compare((*it).name) == 0 ){
            table.erase(it);
            found = true;
            break;
        }
    }
    if( ! found)
        return false;

    // write it back into the string table
    saveRecordTable(table);

    return true;
}

void ResourceFile::loadRecordTable(std::vector<ResourceRecord> & v)
{
    ResourceRecord record;

    v.clear();
    for(unsigned int i = 0; i < m_header.resourceCount; ++i ) {
        m_file.seekg(recordLocation(i), std::ios_base::beg);
        m_file.read((char*)&record, sizeof(record));
        v.push_back(record);
    }
}

unsigned long int ResourceFile::fileSize()
{
    m_file.seekg(0, std::ios::end);
    return m_file.tellg();
}

void ResourceFile::saveRecordTable(std::vector<ResourceRecord> & v)
{
    m_header.resourceCount = v.size();

    // if we don't have enough room, we need to allocate more
    if( recordLocation(m_header.resourceCount) > m_header.dataStart ) {
        // pad for double the current size
        unsigned long int newMaxCount = m_header.resourceCount * 2;
        unsigned long int offset = newMaxCount * sizeof(ResourceRecord);

        // move all data down by offset
        unsigned long int dataSize = fileSize()-m_header.dataStart;

        char * data = new char[dataSize]; // ow, my memory!
        m_file.seekg(m_header.dataStart, std::ios::beg);
        m_file.read(data, dataSize);

        // write the data to the new offset 
        m_header.dataStart += offset;
        m_file.seekp(m_header.dataStart, std::ios::beg);
        m_file.write(data, dataSize);

        delete[] data;

        // adjust the offsets in the record table
        for(unsigned int i = 0; i < v.size(); ++i) {
            v[i].offset += offset;
        }
    }

    // write changes to the header
    m_file.seekp(0, std::ios::beg);
    m_file.write((char*)&m_header, sizeof(m_header));

    // write resource table
    ResourceRecord record;
    for(unsigned int i = 0; i < m_header.resourceCount; ++i ) {
        m_file.seekp(recordLocation(i), std::ios_base::beg);
        record = v.at(i);
        m_file.write((char*)&record, sizeof(record));
    }
}

