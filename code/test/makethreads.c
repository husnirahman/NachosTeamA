#include "syscall.h"

void print(void* ch)
{
    //char *c = (char*)(ch);
    char p = 'a';
    int i;
    for (i = 0; i < 1; i++) {
        PutChar(p+i);
    }
    PutChar('\n');
}
int main()
{
    //do_UserThreadCreate(int f, int arg);
    char c = 'a';
    //void* p = &c;
    UserThreadCreate(print, (void*) &c);
    
     UserThreadExit();
    
    //while(counter>0 ){Yield();}
    //PutChar('\n');
    Halt();
    return 0;
}
