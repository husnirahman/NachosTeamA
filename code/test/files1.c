#include "syscall.h"

int main(){
	createDir("Folder1");
	changeDir("./Folder1/");
	createDir("Folder2");
    changeDir("../");
	changeDir("./Folder1/Folder2/");
	createDir("Folder3");    
    changeDir("../../");
	
	//PutInt(m);
	/*changeDir("..");
	changeDir("..");
	changeDir(".");	
	//displayDir();*/
	return 0;
}
