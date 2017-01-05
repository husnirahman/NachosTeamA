#include "syscall.h"

void sprint(char *s){
    int i = 0;
    while(s[i]!='\0'){
        i++;
    }
    //fprintf(stderr,"size = %d\n", i);
    Puts(s,i);
}
int main()
{
    
    sprint("123");
    Halt();
}