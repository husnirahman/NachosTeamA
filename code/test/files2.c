#include "syscall.h"

int main(){
	int i;
	int n = fopen("small");
	PutInt(n);
	char s[45];
	fread("small",s,45);
	Puts("Reading\n");
	for (i = 0; i<45 ; i++)
		PutChar(s[i]);
    
	char* r = "ABCD";
	Puts("Writing\n");
	fwrite("small", r,0, 4);
	
	char res[38];
	Puts("Reading after writing\n");
	fread("small", res, 38);
	for (i = 0; i<38 ; i++)
		PutChar(res[i]);
    
    fclose("small");
    /*
    char t[45];
    fread("small",t,15);
	Puts("Reading after writing\n");
	for (i = 0; i<15 ; i++)
		PutChar(t[i]);*/
		
	return 0;
}
