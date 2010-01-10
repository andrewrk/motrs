#include "ResourceFile.h"

#include <algorithm>
#include <cstring>

const unsigned long int ResourceFile::initialMaxResources = 100;
const unsigned long int ResourceFile::extraBufferSpace = 4*1024;

ResourceFile::ResourceFile(std::string fileName) :
    m_state(StateUninitialized),
    m_fileName(),
    m_file(),
    m_records()
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

    updateRecordCache();
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
    if( m_file.is_open() )
        m_file.close();
    m_file.clear();
    m_state = StateUninitialized;
}

// return a pointer to the ResourceRecord, or NULL if it can't find it
ResourceFile::ResourceRecord * ResourceFile::getResourceRecord(
    std::string & resourceName)
{
    return &getCacheEntry(resourceName)->record;
}

// return a pointer to the CacheEntry, or NULL if it can't find it
ResourceFile::CacheEntry * ResourceFile::getCacheEntry(
    std::string & resourceName)
{
    std::map<std::string, CacheEntry>::iterator it =
        m_records.find(resourceName);
    
    if( it == m_records.end())
        return NULL;
    
    return &it->second;
}

time_t ResourceFile::getResourceTime(std::string resourceName)
{
    ResourceRecord * record = getResourceRecord(resourceName);

    if( record == NULL )
        return -1;
    
    return record->dateModified;
}

char * ResourceFile::getResource(std::string resourceName)
{
    ResourceRecord * record = getResourceRecord(resourceName);

    if( ! record )
        return NULL;

    m_file.seekg(record->offset, std::ios::beg);
    char * buffer = new char[record->size];
    m_file.read(buffer, record->size);

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
    CacheEntry * entry = getCacheEntry(resourceName);
    ResourceRecord * record = &entry->record;
    
    if( record == NULL ) {
        // add the resource instead
        addResource(resourceName, data, dataSize, dateModified);
        return;
    }

    // if we have enough room to replace the data, do it
    if( record->bufferSize >= dataSize ) {
        m_file.seekp(record->offset, std::ios::beg);
        m_file.write(data, dataSize);

        // write the new data size and modified
        record->size = dataSize;
        record->dateModified = dateModified;
        m_file.seekp(entry->offset, std::ios::beg);
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

    updateRecordCache();
}

void ResourceFile::updateRecordCache()
{
    CacheEntry entry;
    m_records.clear();

    for(unsigned int i = 0; i < m_header.resourceCount; ++i ) {
        entry.offset = recordLocation(i);

        m_file.seekg(entry.offset, std::ios::beg);
        m_file.read((char*)&entry.record, sizeof(entry.record));

        m_records[std::string(entry.record.name)] = entry;
    }
}

void ResourceFile::printNames() {
    updateRecordCache();
    for (std::map<std::string, CacheEntry>::const_iterator it = m_records.begin(); it != m_records.end(); ++it)
        std::cout << it->first << std::endl;
}

