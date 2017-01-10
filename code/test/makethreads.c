#include "syscall.h"

void print(void* ch)
{
    char* p = (char*)ch;
    PutChar(*p);
    //PutChar('\n');
    PutChar(GetChar());
    UserThreadExit();
}

int main()
{
  //// void* p = c;
    char c = GetChar();
    UserThreadCreate(print, (void*)&c);
    
    Join(1);
    Halt();
    return 0;
}
