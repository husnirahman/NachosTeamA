#include "syscall.h"

int main(){
	int i;
	int fp = fopen("small");
	PutInt(fp);
	char s[38];
	fread(fp,s,38);
	Puts("Reading\n");
	for (i = 0; i<38 ; i++)
		PutChar(s[i]);
    
    fclose(fp);
    /*
    char t[45];
    fread("small",t,15);
	Puts("Reading after writing\n");
	for (i = 0; i<15 ; i++)
		PutChar(t[i]);*/
		
	return 0;
}
