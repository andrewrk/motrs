#include "ResourceFile.h"

#include <iostream>
#include <vector>
#include <ctime>
#include <iostream>
#include <fstream>
#include <cstdlib>
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

    if( argc < 2 ) {
        printUsage(argv[0]);
        exit(1);
    }

    // split input
    string command(argv[1]);

    if( command.compare("update") == 0 ) {
        if( argc != 4){
            printUsage(argv[0]);
            exit(1);
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
        if( ! dat.isOpen() ) {
            // try creating the file
            dat.createNew(datfile);
            if( ! dat.isOpen() ) {
                cerr << "Error opening " << datfile << endl;
                exit(1);
            }
        }
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
    } else if( command.compare("list") == 0 ) {
        if( argc != 3){
            printUsage(argv[0]);
            exit(1);
        }

        string datfile(argv[2]);

        ResourceFile dat(datfile);
        if( ! dat.isOpen() ) {
            cerr << "Error opening " << datfile << endl;
            exit(1);
        }

        dat.printNames();
    } else if( command.compare("extract") == 0 ) {
        if( argc != 4 ){
            printUsage(argv[0]);
            exit(1);
        }

        string datfile(argv[2]);
        string infile(argv[3]);

        ResourceFile dat(datfile);
        if( ! dat.isOpen() ) {
            cerr << "Error opening " << datfile << endl;
            exit(1);
        }

        char * buffer = dat.getResource(infile);

        if( buffer == NULL ) {
            cerr << "Resource " << infile << " not found in "
                << datfile << endl;
            exit(1);
        }

        // write it to a file
        ofstream out;
        out.open(infile.c_str(), ios::out | ios::binary | ios::trunc);

        if( ! out.good() ) {
            cerr << "Unable to write to " << infile << endl;
            delete[] buffer;
            exit(1);
        }

        out.write(buffer, dat.resourceSize(infile));
        out.close();

        delete[] buffer;
    } else if( command.compare("squeeze") == 0 ) {
        if( argc != 3){
            printUsage(argv[0]);
            exit(1);
        }

        string datfile(argv[2]);

        ResourceFile dat(datfile);
        if( ! dat.isOpen() ) {
            cerr << "Error opening " << datfile << endl;
            exit(1);
        }

        dat.squeeze();

    } else {
        cout << "command not recognized: " << command << endl;
        printUsage(argv[0]);
        return 1;
    }




    return 0;
}

void printUsage(char * arg0)
{
    cout << "Usage: \n\n";
    cout << arg0 << " update <resource-file> <file>\n";
    cout << "makes sure <file> is updated and in <resource-file>.\n\n";

    cout << arg0 << " extract <resource-file> <file>\n";
    cout << "extracts <file> from <resource-file>\n\n";

    cout << arg0 << " list <resource-file>\n";
    cout << "prints a list of the files in <resource-file>\n\n";

    cout << arg0 << " squeeze <resource-file>\n";
    cout << "removes all wasted buffer space from <resource-file>\n\n";
}

string fileTitle(string fullPath)
{
    int pos_forward = fullPath.rfind('/');
    int pos_back = fullPath.rfind('\\');
    int pos = pos_forward > pos_back ? pos_forward : pos_back;
    return fullPath.substr(pos+1);
}

