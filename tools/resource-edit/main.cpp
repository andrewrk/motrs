#include "ResourceFile.h"

#include <iostream>
#include <vector>
#include <ctime>
#include <iostream>
#include <fstream>
using namespace std;

#include <sys/stat.h>
#include <unistd.h>

void printUsage(char * arg0);
string fileTitle(string fullPath);

int main(int argc, char * argv[])
{
    if( argc == 0 ) {
        cerr << "Who am I?" << endl;
        return 1;
    }

    if( argc < 4 ) {
        printUsage(argv[0]);
        return 1;
    }

    // split input
    string command(argv[1]);

    if( command.compare("update") != 0 ) {
        printUsage(argv[0]);
        return 1;
    }

    string datfile(argv[2]);
    string infile(argv[3]);
    
    // get the modification date of the file on disk
    struct tm * clock;
    struct stat attrib;
    stat(infile.c_str(), &attrib);
    clock = gmtime(&(attrib.st_mtime));
    time_t diskModified = mktime(clock);
    
    // get the modification date of the resource in the resource file
    ResourceFile dat(datfile);
    string infileTitle = fileTitle(infile);
    time_t resourceModified = dat.getResourceTime(infileTitle);

    // if the disk file is newer, update it
    if( diskModified > resourceModified ) {
        cout << "Updating " << infileTitle << "...\n";

        // load the disk content into memory
        ifstream in(infile.c_str());
        in.seekg(0, ios::end);
        unsigned long int length = in.tellg();
        in.seekg(0, ios::beg);

        char * data = new char[length];
        in.read(data, length);
        dat.updateResource(infileTitle, data, length, diskModified);
        delete[] data;
    }
    
    return 0;
}

void printUsage(char * arg0)
{
    cout << "Usage: \n\n";
    cout << arg0 << " update <resource-file> <files>\n\n";
    cout << "makes sure <file> is updated and in <resource-file>.\n";
}

string fileTitle(string fullPath)
{
    size_t pos = fullPath.rfind('/');
    return fullPath.substr(pos+1);
}

