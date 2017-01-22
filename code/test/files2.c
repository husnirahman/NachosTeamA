#include "syscall.h"

int main(){
	
	int n = fopen("small");
	PutInt(n);
	char s[5];
	fread("small",s,5);
	Puts("Reading\n");
	for (int i = 0; i<5 ; i++)
		PutChar(s[i]);
		
	char* r = "ABCD";
	Puts("Writing\n");
	fwrite("small", r, 4);
	
	char res[42];
	Puts("Reading after writing");
	fread("small", res, 42);
	for (int i = 0; i<42 ; i++)
		PutChar(res[i]);
		
	return 0;
}
