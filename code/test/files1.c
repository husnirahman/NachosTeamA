#include "syscall.h"

int main(){
	createDir("Folder1");
	changeDir("Folder1");
	createDir("Folder2");
	changeDir("Folder2");
	createDir("Folder3");
	
	//PutInt(m);
	changeDir("..");
	changeDir("..");
	changeDir(".");	
	//displayDir();
	return 0;
}
