#include "syscall.h"

void print(void* ch)
{
    //char *c = (char*)(ch);
    char p = 'a';
    PutChar(p);
    PutChar('\n');
   UserThreadExit();
}
int main()
{
    //do_UserThreadCreate(int f, int arg);
    char c = 'a';
    //void* p = &c;
    UserThreadCreate(print, (void*) &c);
    
    //PutInt(n);
    //UserThreadExit();
    
    Puts("help\n");
    
    //while(counter>0 ){Yield();}
    //PutChar('\n');
    //GetChar();
    Halt();
    return 0;
}
