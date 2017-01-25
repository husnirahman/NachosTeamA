#include "syscall.h"

int main(){
    
    fcreate("file1", 10000);
    fopen("file1");
    fseek(0,3000);
    
    char c[4000];
    int i;
    for(i = 0 ; i < 4000; i++)
        c[i] = 'a';
    
    fwrite(0, c, 4000);

    //PutInt(id);
    fclose(0);
    
    fopen("file1");
    char s[4000];
    fseek(0,2995);
	fread(0,s,1000);
	Puts("Reading\n");
	for (i = 0; i<1005 ; i++){
		PutChar(s[i]);PutChar('b');
	}
    
    PutInt(i);
    fclose(0);
    return 0;
}
