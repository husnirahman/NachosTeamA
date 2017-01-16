#include "syscall.h"

int main()
{	
	//Puts("main2 \n");
	
	ForkExec("./userpages0a");
	//PutChar('Z');
	ForkExec("./userpages1a");
	
	char n = GetChar();
	PutChar(n);
	return 0;
}
