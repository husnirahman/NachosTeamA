#include "syscall.h"

void print(void* ch)
{
    char* p = (char*)ch;
    PutChar(*p);
    PutChar('\n');
    PutChar(GetChar());
    PutChar(GetChar());
    UserThreadExit();
}

int main()
{
  //// void* p = c;
    char c = GetChar();
    int n = UserThreadCreate(print, (void*)&c);
    PutInt(n);
    Join(1);
    Halt();
    return 0;
}
