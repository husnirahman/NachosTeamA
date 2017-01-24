#include "syscall.h"

int main(){
    
    PutInt(fcreate("file1", 4000));
    PutInt(fopen("file1"));
    PutInt(fseek(0,0));
    
    char c[4000];
    int i;
    for(i = 0 ; i < 4000; i++)
        c[i] = 'a';
    
    PutInt(fwrite(0, c, 4000));
    
    char s[4000];
    fseek(0,0);
	fread(0,s,4000);
	Puts("Reading\n");
	for (i = 0; i<4000 ; i++)
		PutChar(s[i]);
    
    PutInt(i);
    fclose(0);
    return 0;
}