#include "ResourceFile.h"

#include <algorithm>
#include <cstring>

const unsigned long int ResourceFile::initialMaxResources = 100;
const unsigned long int ResourceFile::extraBufferSpace = 4*1024;

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

    m_file.open(m_fileName.c_str(),
        std::fstream::in | std::fstream::out | std::fstream::binary);
    if( ! m_file.good() ) {
        m_state = StateError;
        return;
    }

    // header
    m_file.read((char*)&m_header, sizeof(m_header));

    m_state = StateReady;
}

void ResourceFile::createNew(std::string fileName)
{
    close();
    m_fileName = fileName;

    // create the file
    m_file.open(m_fileName.c_str(),
        std::fstream::out | std::fstream::binary | std::fstream::trunc);
    m_file.close();
    
    m_file.open(m_fileName.c_str(),
        std::fstream::in | std::fstream::out | std::fstream::binary);
    
    if( ! m_file.good() ){
        m_state = StateError;
        return;
    }
    
    // write the initial resource table
    // leave room for extra resource names
    m_header.dataStart = recordLocation(initialMaxResources+1);
    m_header.resourceCount = 0;
    m_file.write((char *)&m_header, sizeof(m_header));
    
    // write all that padding until data start
    unsigned long int paddingSize = m_header.dataStart - sizeof(m_header);
    char * padding = new char[paddingSize];
    m_file.write(padding, paddingSize);
    delete[] padding;

    m_state = StateReady;
}

void ResourceFile::close()
{
    if( m_state == StateReady ) {
        m_file.close();
    }
    m_state = StateUninitialized;
}

// return the resource index of the string
int ResourceFile::findResourceRecord(std::string resourceName)
{
    // use the binary search algorithm to find the location
    int left = 0;
    int middle = -1;
    int right = m_header.resourceCount-1;
    ResourceRecord record;
    int cmp;

    while(right-left >= 0) {
        middle = (right + left) / 2; 
        
        // get the string at this location
        m_file.seekg(recordLocation(middle), std::ios::beg);
        m_file.read((char*)&record, sizeof(record));
        
        cmp = resourceName.compare(record.name);

        if( cmp > 0 ) {
            left = middle+1;
        } else if( cmp < 0 ) {
            right = middle-1;
        } else {
            // found it!
            return middle;
        }
    }
    // can't find it
    return -1;
}

time_t ResourceFile::getResourceTime(std::string resourceName)
{
    int index = findResourceRecord(resourceName);
    if( index == -1 )
        return -1;
    ResourceRecord record;
    m_file.seekg(recordLocation(index), std::ios::beg);
    m_file.read((char*)&record, sizeof(record));
    
    return record.dateModified;
}

char * ResourceFile::getResource(std::string resourceName)
{
    int index = findResourceRecord(resourceName);
    if( index == -1 )
        return NULL;
    ResourceRecord record;
    m_file.seekg(recordLocation(index), std::ios::beg);
    m_file.read((char*)&record, sizeof(record));

    m_file.seekg(record.offset, std::ios::beg);
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

void ResourceFile::updateResource(std::string resourceName, const char * data,
    unsigned long int dataSize, time_t dateModified)
{
    // find the resource
    int recordIndex = findResourceRecord(resourceName);
    if( recordIndex == -1 ) {
        // add instead
        addResource(resourceName, data, dataSize, dateModified);
        return;
    }

    ResourceRecord record;
    m_file.seekg(recordLocation(recordIndex), std::ios::beg);
    m_file.read((char*)&record, sizeof(record));

    // if we have enough room to replace the data, do it
    if( record.bufferSize >= dataSize ) {
        m_file.seekp(record.offset, std::ios::beg);
        m_file.write(data, dataSize);

        // write the new data size and modified
        record.size = dataSize;
        record.dateModified = dateModified;
        m_file.seekp(recordLocation(recordIndex), std::ios::beg);
        m_file.write((char*)&record, sizeof(record));
    } else {
        // not enough room, simply delete the resource and add the new one
        deleteResource(resourceName);
        addResource(resourceName, data, dataSize, dateModified);
    }
}

void ResourceFile::addResource(std::string resourceName, const char * data,
    unsigned long int dataSize, time_t dateModified)
{
    // load the string table into a vector
    std::vector<ResourceRecord> table;
    loadRecordTable(table);

    // add the new resource record
    m_file.seekp(0, std::ios::end);

    ResourceRecord record;
    std::strcpy(record.name, resourceName.c_str());
    record.size = dataSize;
    record.dateModified = dateModified;
    record.bufferSize = dataSize + extraBufferSpace;
    record.offset = m_file.tellp();

    table.push_back(record);

    // write the resource to the end of the file
    m_file.write(data, dataSize);

    // write the extra buffer space
    char * trash = new char[record.bufferSize-dataSize];
    m_file.write(trash, record.bufferSize-dataSize);
    delete[] trash;
    
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
    std::vector<ResourceRecord>::iterator prev;
    bool first = true;
    for( std::vector<ResourceRecord>::iterator it=table.begin();
        it < table.end(); ++it)
    {
        if( resourceName.compare((*it).name) == 0 ){
            // give the buffer space to the resource before this one
            if( ! first )
                (*prev).bufferSize += (*it).bufferSize;

            // erase this one
            table.erase(it);
            found = true;
            break;
        }
        prev = it;
        first = false;
    }
    if( ! found)
        return false;

    // write it back into the string table
    saveRecordTable(table);

    return true;
}

void ResourceFile::squeeze()
{
    throw "TODO: get rid of extra buffer space";
}

void ResourceFile::loadRecordTable(std::vector<ResourceRecord> & v)
{
    ResourceRecord record;

    v.clear();
    for(unsigned int i = 0; i < m_header.resourceCount; ++i ) {
        m_file.seekg(recordLocation(i), std::ios::beg);
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
        m_file.seekp(recordLocation(i), std::ios::beg);
        record = v.at(i);
        m_file.write((char*)&record, sizeof(record));
    }
}

