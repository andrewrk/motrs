#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/param.h>
 
#ifndef O_BINARY
#define O_BINARY 0x8000
#endif

typedef struct {
	char* filename;
	char* fullpath;
} fileToPack;

//Function prototypes
int getfilesize(char *filename);
int countfiles(char *path);
void packfile(char* fullpath, char *filename, int fd) ;
void findfiles(char *path, int fd);
int compare( const void* a, const void* b);
//void qsort (fileToPack* array, int left, int right);
//int partition(fileToPack* array, int left, int right, int pivotIndex);
//void swap( fileToPack* array, int index1, int index2);
 
int currentfile = 1;	//This integer indicates what file we're currently adding to the resource.
int currentloc = 0;	//This integer references the current write-location within the resource file

fileToPack* files; //array of files to pack. sort this and then pack.
int filesIndex = 0;

int main(int argc, char *argv[]) {
	
	if( argc < 2 ){
		printf("\nUsage:\n\nsjsresource <resourcefolder> <outfile>\n\n");
		exit(1);
	}
	
	int i;
	
	char pathname[MAXPATHLEN+1];	//This character array will hold the app's working directory path
	int filecount;			//How many files are we adding to the resource?
	int fd;				//The file descriptor for the new resource
	
	//Store the current path
	getcwd(pathname, sizeof(pathname));
	
	//How many files are there?
	filecount = countfiles(argv[1]);
	//printf("NUMBER OF FILES: %i\n", filecount); STFU
	
	files = malloc(sizeof(fileToPack) * filecount);
	
	//Go back to the original path
	chdir(pathname);
	
	//How many arguments did the user pass?
	if (argc < 3)
	{
		//The user didn't specify a resource file name, go with the default
		fd = open("resource.dat", O_WRONLY | O_CREAT | O_BINARY, S_IRUSR | S_IWUSR);
	}
	else
	{
		//Use the filename specified by the user
		remove(argv[2]);
		fd = open(argv[2], O_WRONLY | O_CREAT | O_BINARY, S_IRUSR | S_IWUSR);
	}
	//Did we get a valid file descriptor?
	if (fd < 0) 
	{
		//Can't create the file for some reason (possibly because the file already exists)
		perror("Cannot create resource file");
		exit(1);
	}
		
	//Write the total number of files as the first integer
	write(fd, &filecount, sizeof(int));
	
	//Set the current conditions
	currentfile = 1;					//Start off by storing the first file, obviously!
	currentloc = (sizeof(int) * filecount) + sizeof(int);	//Leave space at the begining for the header info
	
	//Use the findfiles routine to get an array of the files
	filesIndex = 0;
	findfiles(argv[1], fd);
	
	
	
	//sort the array
	qsort(files, filecount, sizeof(fileToPack), compare);
	
	//pack the files
	for(i=0; i<filecount; i++){
		packfile(files[i].fullpath, files[i].filename,  fd);
	}
	
	//Close the file
	close(fd);
	
	return 0;
}

int compare( const void* a, const void* b){
	return strcmp(  ((fileToPack*)a)->filename, ((fileToPack*)b)->filename   );
}

int getfilesize(char *filename ){
	struct stat file;
	
	if( ! stat(filename, &file) )
		return file.st_size;
	
	//crash
	fprintf(stderr, "getfilesize: Couldn't get the filesize of %s.\n", filename);
	exit(1);
}

int countfiles(char *path) {
	
	int count = 0;			//This integer will count up all the files we encounter
	struct dirent *entry;		//This structure will hold file information
	struct stat file_status;	//This structure will be used to query file status
	DIR *dir = opendir(path);	//This pointer references the directory stream
		
	//Make sure we have a directory stream pointer
	if (!dir) {
		perror("opendir failure");
		exit(1);
	}
	
	//Change directory to the given path
	chdir(path);
	
	//Loop through all files and directories
	while ( (entry = readdir(dir)) != NULL) {
		//Don't bother with directories that start with .
		if ( entry->d_name[0] != '.' ) {
			//Get the status info for the current file
			if (stat(entry->d_name, &file_status) == 0) {
				//Is this a directory, or a file?
				if (S_ISDIR(file_status.st_mode)) {
					//Call countfiles again (recursion) and add the result to the count total
					count += countfiles(entry->d_name);
					chdir("..");
				}
				else {
					//We've found a file, increment the count
					count++;
				}
			}
		}
	}
	
	//Make sure we close the directory stream
	if (closedir(dir) == -1) {
		perror("closedir failure");
		exit(1);
	}
	
	//Return the file count
	return count;
}

void findfiles(char *path, int fd) {
	
	struct dirent *entry;		//This structure will hold file information
	struct stat file_status;	//This structure will be used to query file status
	DIR *dir = opendir(path);	//This pointer references the directory stream
		
	//Make sure we have a directory stream pointer
	if (!dir) {
		perror("opendir failure");
		exit(1);
	}
	
	//Change directory to the given path
	chdir(path);
	
	//Loop through all files and directories
	while ( (entry = readdir(dir)) != NULL) {
		//Don't bother with the directories that start with .
		if ( entry->d_name[0] != '.' ) {
			//Get the status info for the current file
			if (stat(entry->d_name, &file_status) == 0) {
				//Is this a directory, or a file?
				if (S_ISDIR(file_status.st_mode)) {
					//Call findfiles again (recursion), passing the new directory's path
					findfiles(entry->d_name, fd);
					chdir("..");
				}
				else {
					//We've found a file, add it to an array for sorting
					char pathname[MAXPATHLEN+1];
					getcwd(pathname, sizeof(pathname));
					
					//create a filetopack entry
					fileToPack pfile;
					pfile.filename = malloc(sizeof(char)*(MAXPATHLEN+1));
					pfile.fullpath = malloc(sizeof(char)*(MAXPATHLEN+1));
					
					strcpy(pfile.filename, entry->d_name);
					strcpy(pfile.fullpath, pathname);
					strcat(pfile.fullpath, "/");
					strcat(pfile.fullpath, entry->d_name);
					
					//add to array
					files[filesIndex] = pfile;
					filesIndex++;
					
				}
			}
		}
	}
	
	//Make sure we close the directory stream
	if (closedir(dir) == -1) {
		perror("closedir failure");
		exit(1);
	}
	
	return;	
}


void packfile(char* fullpath, char *filename, int fd) {
	
	int totalsize = 0;	//This integer will be used to track the total number of bytes written to file
	
	//Handy little output (if you have nothing alarming to say, shut the fuck up)
	//printf("PACKING: '%s' SIZE: %i\n", filename, getfilesize(fullpath));
	
	//In the 'header' area of the resource, write the location of the file about to be added
	lseek(fd, currentfile * sizeof(int), SEEK_SET);
	write(fd, &currentloc, sizeof(int));
	
	//Seek to the location where we'll be storing this new file info
	lseek(fd, currentloc, SEEK_SET);
	
	//Write the size of the file
	int filesize = getfilesize(fullpath);
	write(fd, &filesize, sizeof(filesize));
	totalsize += sizeof(int);
	
	//Write the LENGTH of the NAME of the file
	int filenamelen = strlen(filename);
	write(fd, &filenamelen, sizeof(int));
	totalsize += sizeof(int);
	
	//Write the name of the file
	write(fd, filename, strlen(filename));
	totalsize += strlen(filename);
	
	//Write the file contents
	int fd_read = open(fullpath, O_RDONLY);		//Open the file
	char *buffer = malloc(filesize);	//Create a buffer for its contents
	read(fd_read, buffer, filesize);		//Read the contents into the buffer
	write(fd, buffer, filesize);			//Write the buffer to the resource file
	
	close(fd_read);					//Close the file
	free(buffer);					//Free the buffer
	totalsize += filesize;				//Add the file size to the total number of bytes written
	
	//Increment the currentloc and current file values
	currentfile++;
	currentloc += totalsize;
	
	//lseek(fd, currentloc, SEEK_SET); //overwrite the evil end of line bytes
}


