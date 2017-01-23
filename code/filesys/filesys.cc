// filesys.cc 
//	Routines to manage the overall operation of the file system.
//	Implements routines to map from textual file names to files.
//
//	Each file in the file system has:
//	   A file header, stored in a sector on disk 
//		(the size of the file header data structure is arranged
//		to be precisely the size of 1 disk sector)
//	   A number of data blocks
//	   An entry in the file system directory
//
// 	The file system consists of several data structures:
//	   A bitmap of free disk sectors (cf. bitmap.h)
//	   A directory of file names and file headers
//
//      Both the bitmap and the directory are represented as normal
//	files.  Their file headers are located in specific sectors
//	(sector 0 and sector 1), so that the file system can find them 
//	on bootup.
//
//	The file system assumes that the bitmap and directory files are
//	kept "open" continuously while Nachos is running.
//
//	For those operations (such as Create, Remove) that modify the
//	directory and/or bitmap, if the operation succeeds, the changes
//	are written immediately back to disk (the two files are kept
//	open during all this time).  If the operation fails, and we have
//	modified part of the directory and/or bitmap, we simply discard
//	the changed version, without writing it back to disk.
//
// 	Our implementation at this point has the following restrictions:
//
//	   there is no synchronization for concurrent accesses
//	   files have a fixed size, set when the file is created
//	   files cannot be bigger than about 3KB in size
//	   there is no hierarchical directory structure, and only a limited
//	     number of files can be added to the system
//	   there is no attempt to make the system robust to failures
//	    (if Nachos exits in the middle of an operation that modifies
//	    the file system, it may corrupt the disk)
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"

#include "disk.h"
#include "bitmap.h"
#include "directory.h"
#include "filehdr.h"
#include "filesys.h"
#ifdef FILESYS
#include "synch.h"
#define MAX_PATH_SIZE 100
#endif //FILESYS

// Sectors containing the file headers for the bitmap of free sectors,
// and the directory of files.  These file headers are placed in well-known 
// sectors, so that they can be located on boot-up.
#define FreeMapSector 		0
#define DirectorySector 	1

// Initial file sizes for the bitmap and directory; until the file system
// supports extensible files, the directory size sets the maximum number 
// of files that can be loaded onto the disk.
#define FreeMapFileSize 	(NumSectors / BitsInByte)
#define NumDirEntries 		10
#define DirectoryFileSize 	(sizeof(DirectoryEntry) * NumDirEntries)
#ifdef CHANGED
#define NumFileEntries 		10
#endif //CHANGED

Lock* lockTable = new Lock("lockTable");
Condition* condTable = new Condition("condTable");

//----------------------------------------------------------------------
// FileSystem::FileSystem
// 	Initialize the file system.  If format = TRUE, the disk has
//	nothing on it, and we need to initialize the disk to contain
//	an empty directory, and a bitmap of free sectors (with almost but
//	not all of the sectors marked as free).  
//
//	If format = FALSE, we just have to open the files
//	representing the bitmap and the directory.
//
//	"format" -- should we initialize the disk?
//----------------------------------------------------------------------

FileSystem::FileSystem(bool format)
{ 
    DEBUG('f', "Initializing the file system.\n");
    if (format) {
        BitMap *freeMap = new BitMap(NumSectors);
        Directory *directory = new Directory(NumDirEntries);
		FileHeader *mapHdr = new FileHeader;
		FileHeader *dirHdr = new FileHeader;

        DEBUG('f', "Formatting the file system.\n");

		// First, allocate space for FileHeaders for the directory and bitmap
		// (make sure no one else grabs these!)
		freeMap->Mark(FreeMapSector);	    
		freeMap->Mark(DirectorySector);

		// Second, allocate space for the data blocks containing the contents
		// of the directory and bitmap files.  There better be enough space!

		ASSERT(mapHdr->Allocate(freeMap, FreeMapFileSize));
		ASSERT(dirHdr->Allocate(freeMap, DirectoryFileSize));

		// Flush the bitmap and directory FileHeaders back to disk
		// We need to do this before we can "Open" the file, since open
		// reads the file header off of disk (and currently the disk has garbage
		// on it!).

		DEBUG('f', "Writing headers back to disk.\n");
		mapHdr->WriteBack(FreeMapSector);    
		dirHdr->WriteBack(DirectorySector);

		// OK to open the bitmap and directory files now
		// The file system operations assume these two files are left open
		// while Nachos is running.

		freeMapFile = new OpenFile(FreeMapSector);
        directoryFile = new OpenFile(DirectorySector);
     
		// Once we have the files "open", we can write the initial version
		// of each file back to disk.  The directory at this point is completely
		// empty; but the bitmap has been changed to reflect the fact that
		// sectors on the disk have been allocated for the file headers and
		// to hold the file data for the directory and bitmap.

		DEBUG('f', "Writing bitmap and directory back to disk.\n");
		freeMap->WriteBack(freeMapFile);	 // flush changes to disk
		directory->WriteBack(directoryFile);
		
		if (DebugIsEnabled('f')) {
			freeMap->Print();
			directory->Print();

		    delete freeMap; 
			delete directory; 
			delete mapHdr; 
			delete dirHdr;
		}
#ifdef CHANGED
		
		DEBUG('f', "Creating dot directing  in root directory\n");
	
		Directory* dot = new Directory(NumDirEntries);
		dot->FetchFrom(directoryFile);
		dot->Add(".", DirectorySector);
		dot->WriteBack(directoryFile);
		dot->Add("..", DirectorySector);
		dot->WriteBack(directoryFile);
		delete dot;
		
		OpenFile* file = new OpenFile(DirectorySector);
		Directory* root = new Directory(NumDirEntries);
		root->FetchFrom(file);
		
		printf("Sector of root dir in (.) dir = %d \n", root->Find("."));
		printf("Sector of root dir in (..) dir = %d \n", root->Find(".."));
		delete root;
		delete file;
    	currOpenFile = new OpenFile(DirectorySector);
		
#endif //CHANGED
    } 
    else {
    // if we are not formatting the disk, just open the files representing
    // the bitmap and directory; these are left open while Nachos is running
        freeMapFile = new OpenFile(FreeMapSector);
        directoryFile = new OpenFile(DirectorySector);
#ifdef CHANGED 
        currOpenFile = new OpenFile(DirectorySector);
#endif//CHANGED
    }
#ifdef CHANGED  
    	table = new FileEntry[NumFileEntries];   //Initializing open files table
		int i;
		for (i = 0; i < NumFileEntries; i++)
        	table[i].inUse = FALSE;
#endif//CHANGED

}

//----------------------------------------------------------------------
// FileSystem::Create
// 	Create a file in the Nachos file system (similar to UNIX create).
//	Since we can't increase the size of files dynamically, we have
//	to give Create the initial size of the file.
//
//	The steps to create a file are:
//	  Make sure the file doesn't already exist
//        Allocate a sector for the file header
// 	  Allocate space on disk for the data blocks for the file
//	  Add the name to the directory
//	  Store the new file header on disk 
//	  Flush the changes to the bitmap and the directory back to disk
//
//	Return TRUE if everything goes ok, otherwise, return FALSE.
//
// 	Create fails if:
//   		file is already in directory
//	 	no free space for file header
//	 	no free entry for file in directory
//	 	no free space for data blocks for the file 
//
// 	Note that this implementation assumes there is no concurrent access
//	to the file system!
//
//	"name" -- name of file to be created
//	"initialSize" -- size of file to be created
//----------------------------------------------------------------------

bool
FileSystem::Create(const char *name, int initialSize)
{
    Directory *directory;
    BitMap *freeMap;
    FileHeader *hdr;
    int sector;
    bool success;

    DEBUG('f', "Creating file %s, size %d\n", name, initialSize);

    directory = new Directory(NumDirEntries);
#ifndef CHANGED
    directory->FetchFrom(directoryFile);
#else
	directory->FetchFrom(currOpenFile);
#endif 
    if (directory->Find(name) != -1)
      success = FALSE;			// file is already in directory
    else {	
        freeMap = new BitMap(NumSectors);
        freeMap->FetchFrom(freeMapFile);
        sector = freeMap->Find();	// find a sector to hold the file header
    	if (sector == -1) 		
            success = FALSE;		// no free block for file header 
        else if (!directory->Add(name, sector))
            success = FALSE;	// no space in directory
	else {
    	    hdr = new FileHeader;
	    if (!hdr->Allocate(freeMap, initialSize))
            	success = FALSE;	// no space on disk for data
	    else {	
	    	success = TRUE;
		// everthing worked, flush all changes back to disk
    	    	hdr->WriteBack(sector); 
    	  #ifndef CHANGED		
    	    	directory->WriteBack(directoryFile);
    	   #else
    	   		directory->WriteBack(currOpenFile);
    	  #endif //CHANGED
    	    	freeMap->WriteBack(freeMapFile);
	    }
            delete hdr;
	}
        delete freeMap;
    }
    delete directory;

    return success;
}

//----------------------------------------------------------------------
// FileSystem::Open
// 	Open a file for reading and writing.  
//	To open a file:
//	  Find the location of the file's header, using the directory 
//	  Bring the header into memory
//
//	"name" -- the text name of the file to be opened
//----------------------------------------------------------------------

OpenFile *
FileSystem::Open(const char *name)
{ 
    Directory *directory = new Directory(NumDirEntries);
    OpenFile *openFile = NULL;
    int sector;

    DEBUG('f', "Opening file %s\n", name);
#ifndef CHANGED
	directory->FetchFrom(directoryFile);
#else
    directory->FetchFrom(currOpenFile);
#endif //CHANGED
    sector = directory->Find(name); 
    if (sector >= 0) 		
	openFile = new OpenFile(sector);	// name was found in directory 
    delete directory;
    return openFile;				// return NULL if not found
}

//----------------------------------------------------------------------
// FileSystem::Remove
// 	Delete a file from the file system.  This requires:
//	    Remove it from the directory
//	    Delete the space for its header
//	    Delete the space for its data blocks
//	    Write changes to directory, bitmap back to disk
//
//	Return TRUE if the file was deleted, FALSE if the file wasn't
//	in the file system.
//
//	"name" -- the text name of the file to be removed
//----------------------------------------------------------------------

bool
FileSystem::Remove(const char *name)
{ 
    Directory *directory;
    BitMap *freeMap;
    FileHeader *fileHdr;
    int sector;
    
    directory = new Directory(NumDirEntries);
#ifndef CHANGED
	directory->FetchFrom(directoryFile);
#else
    directory->FetchFrom(currOpenFile);
#endif //CHANGED
    sector = directory->Find(name);
    if (sector == -1) {
       delete directory;
       return FALSE;			 // file not found 
    }
    fileHdr = new FileHeader;
    fileHdr->FetchFrom(sector);

    freeMap = new BitMap(NumSectors);
    freeMap->FetchFrom(freeMapFile);

    fileHdr->Deallocate(freeMap);  		// remove data blocks
    freeMap->Clear(sector);			// remove header block
    directory->Remove(name);

    freeMap->WriteBack(freeMapFile);		// flush to disk
    directory->WriteBack(directoryFile);        // flush to disk
    delete fileHdr;
    delete directory;
    delete freeMap;
    return TRUE;
} 

//----------------------------------------------------------------------
// FileSystem::List
// 	List all the files in the file system directory.
//----------------------------------------------------------------------

void
FileSystem::List()
{
    Directory *directory = new Directory(NumDirEntries);
#ifndef CHANGED
   directory->FetchFrom(directoryFile);
#else
	directory->FetchFrom(currOpenFile);
#endif //CHANGED
    directory->List();
    delete directory;
}

//----------------------------------------------------------------------
// FileSystem::Print
// 	Print everything about the file system:
//	  the contents of the bitmap
//	  the contents of the directory
//	  for each file in the directory,
//	      the contents of the file header
//	      the data in the file
//----------------------------------------------------------------------

void
FileSystem::Print()
{
    FileHeader *bitHdr = new FileHeader;
    FileHeader *dirHdr = new FileHeader;
    BitMap *freeMap = new BitMap(NumSectors);
    Directory *directory = new Directory(NumDirEntries);

    printf("Bit map file header:\n");
    bitHdr->FetchFrom(FreeMapSector);
    bitHdr->Print();

    printf("Directory file header:\n");
    dirHdr->FetchFrom(DirectorySector);
    dirHdr->Print();

    freeMap->FetchFrom(freeMapFile);
    freeMap->Print();

    directory->FetchFrom(directoryFile);
    directory->Print();

    delete bitHdr;
    delete dirHdr;
    delete freeMap;
    delete directory;
} 

#ifdef CHANGED
//----------------------------------------------------------------------
// FileSystem::CreateD(const char* name)
// Create new directory in current location and add (.) and (..) 
// directories
//----------------------------------------------------------------------
bool
FileSystem::CreateD(const char *name){
	Directory *directory;
    BitMap *freeMap;
    FileHeader *hdr;
    int sector;
    bool success;

    DEBUG('f', "Creating directing %s\n", name);

    directory = new Directory(NumDirEntries);
    directory->FetchFrom(currOpenFile);

    if (directory->Find(name) != -1)
      success = FALSE;			// file is already in directory
    else {	
        freeMap = new BitMap(NumSectors);
        freeMap->FetchFrom(freeMapFile);
        sector = freeMap->Find();	// find a sector to hold the file header
        printf("Directory = %s Sector Allocated = %d\n",name, sector);
    	if (sector == -1) 		
            success = FALSE;		// no free block for file header 
        else if (!directory->Add(name, sector))
            success = FALSE;	// no space in directory
		else {
    	    hdr = new FileHeader;
	    	if (!hdr->Allocate(freeMap, DirectoryFileSize))
            	success = FALSE;	// no space on disk for data
	    	else {	
	    		success = TRUE;
				// everthing worked, flush all changes back to disk
    	    	hdr->WriteBack(sector); 		
    	    	directory->WriteBack(currOpenFile);
    	    	freeMap->WriteBack(freeMapFile);
	    	}
            delete hdr;
		}
        delete freeMap;
    }
    delete directory;
    
/* Creating dot directory */

	DEBUG('f', "Creating dot directing \n");
	
	OpenFile* file = new OpenFile(sector);
    Directory* dot = new Directory(NumDirEntries);
    
    dot->FetchFrom(file);
	dot->Add(".", sector);
  	dot->WriteBack(file);

    delete dot;
    delete file;
    
	//checking
    file = new OpenFile(sector);
    Directory* check = new Directory(NumDirEntries);
    check->FetchFrom(file);
    printf("Sector of (.) directory of %s directory  = %d \n", name, check->Find("."));
        
    delete check;
    delete file;
    
/* Creating dot dot directory */
	DEBUG('f', "Creating  dot dot directing \n");
	
	file = new OpenFile(sector);
    Directory* ddot = new Directory(NumDirEntries);
    
    ddot->FetchFrom(file);
    Directory* par = new Directory(NumDirEntries);
    par->FetchFrom(currOpenFile);
    printf("Sector of (..) directory of %s directory = %d\n",name, par->Find("."));
   	ddot->Add("..", par->Find("."));
	ddot->WriteBack(file);

    delete ddot;
    delete file;
    return success;
}

//-------------------------------------------------------------------------
// FileSystem:: ChangeD(const char* name
// Change currOpenFile to the name if it exists in the current directory
// Return TRUE on SUCCESS else FALSE
//----------------------------------------------------------------------
bool interpret(char* path, char* name ){
    int i = 0;
    
    printf("Entering Interpreter... \n");
    if(*path=='\0'){
        return FALSE;
    }
    else{
        char* temp = new char[MAX_PATH_SIZE];
        while(*path != '/'){
            temp[i] = *path;
            printf("%c", temp[i]);
            path++;
            i++;
        }
        printf("\n");
        strncpy(name, temp, MAX_PATH_SIZE);
        printf("Subname in interpret = %s\n", temp);
        delete temp;
        return TRUE;
    }
}
bool
FileSystem:: ChangeD(const char* name){
	Directory* directory = new Directory(NumDirEntries);
	directory->FetchFrom(currOpenFile);
	bool success;
	
    char* path = new char[MAX_PATH_SIZE];
    strncpy(path, name, MAX_PATH_SIZE);
    char* subname = new char[MAX_PATH_SIZE];
    
    while(interpret(path , subname)){
        printf("Subname in open = %s\n", subname);
        
        int sector = directory->Find(subname);
        printf("Change dir:  name = %s and sector = %d\n", name, sector);
	
        if(sector == -1){
            success = FALSE;
            break;
        }
        else{
            success = TRUE;
            delete currOpenFile;
            printf("Checking sector value = %d\n",sector);
            if(sector ==1)
                currOpenFile = new OpenFile(DirectorySector);
            else
                currOpenFile = new OpenFile(sector);
            Directory* temp = new Directory(NumDirEntries);
            temp->FetchFrom(currOpenFile);
            printf("Contents after changing directory\n");
            temp->Print();
        }
    }
	return success;
}

//----------------------------------------------------------------------
// FileSystem::FFindIndex
// 	Open a file  and print its contents
//
//	"name" -- the text name of the file to be opened
//----------------------------------------------------------------------



//----------------------------------------------------------------------
// FileSystem::fileopen
// 	Open a file  and print its contents
//
//	"name" -- the text name of the file to be opened
//----------------------------------------------------------------------
int
FileSystem::fileopen(const char *name)
{ 
	Directory* directory = new Directory(NumDirEntries);
    int sector;
    int i ;
    
    DEBUG('f', "Opening file %s\n", name);
	directory->FetchFrom(currOpenFile);
	sector = directory->Find(name); 
	delete directory;
	
    lockTable->Acquire();
    
    if (sector >= 0) {				
        while (FFindIndex(name) > -1) {
            condTable->Wait(lockTable);
        } 
    
        DEBUG('f', "Not Stuck %s\n", name);
		for (i = 0; i < NumFileEntries; i++){
        	if (!table[i].inUse) {
        		table[i].inUse = TRUE;            	
           		strncpy(table[i].name, name, FileNameMaxLen); 
            	table[i].sector = sector;            	
				OpenFile* openFile = new OpenFile(sector);	// name was found in directory 				
            	table[i].file = openFile;            	
                lockTable->Release();
        		return i;
			}
		}
	}
	
    lockTable->Release();
	return -1;
}

int
FileSystem::FFindIndex(const char *name)
{
    for (int i = 0; i < NumFileEntries; i++)
        if (table[i].inUse && !strncmp(table[i].name, name, FileNameMaxLen))
	    	return i;
    return -1;		// name not in directory
}

int 
FileSystem :: fileread(int fp, char* to, int size){
    lockTable->Acquire();
    
    if(!table[fp].inUse){    
        printf("File not opened\n");
    	to = NULL;
        lockTable->Release();
		return -1;
    }
    
	OpenFile* openFile = table[fp].file;
	
    if(size > openFile->Length()){
        printf("Bytes to be read greater than file size. Reading till enf of file\n");
        size = openFile->Length();
    }
	openFile->Read(to, size);
	
    lockTable->Release();
	return 0;
}

int 
FileSystem :: filewrite(int fp, char* from, int size){
    
    lockTable->Acquire();
    
    if(!table[fp].inUse){    
        printf("File not opened\n");
        lockTable->Release();
		return -1;
    }
    
	OpenFile* openFile = table[fp].file;
	
	printf("Writing file length = %d \n", openFile->Length());	
	openFile->Write(from, size);
    
    lockTable->Release();
	return 0;
}

int
FileSystem :: fileclose(int fp){
    lockTable->Acquire();
    
    if(!table[fp].inUse){    
        printf("File not opened. Therfore cannot be closed\n"); 
        lockTable->Release();
		return -1;
    }   
   
    printf("Closing file = %s\n", table[fp].name);
    table[fp].inUse = FALSE; 
    table[fp].sector = -1;  
    table[fp].file = NULL;      
    condTable->Signal(lockTable);
    lockTable->Release();
    return 0;    
}

int 
FileSystem::fileseek(int fp, int position){    
    lockTable->Acquire();
    
    if(!table[fp].inUse){    
        printf("File not opened.\n"); 
        lockTable->Release();
		return -1;
    }    
    
	OpenFile* openFile = table[fp].file;
    printf("Fseek position= %d \n", position);
    openFile->Seek(position);
    
    lockTable->Release();   
    return 0;
}

#endif //CHANGED
