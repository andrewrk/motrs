#include "ResourceFile.h"

#include <iostream>
#include <vector>
using namespace std;

void printUsage(char * arg0);

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
    string outfile(argv[2]);
    string infile(argv[3]);
    
    return 1;
    
    
    return 0;
}

void printUsage(char * arg0)
{
    cout << "Usage: \n\n";
    cout << arg0 << " update <resource-file> <files>\n\n";
    cout << "makes sure <file> is updated and in <resource-file>.\n";
}


