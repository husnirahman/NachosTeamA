#include "syscall.h"

char scan()
{
   char c = GetChar();
   PutChar(c);
    
    //Puts("two");
    /*
    char v = GetChar();
    PutChar(v);
    
    char s = GetChar();
    PutChar(s);
    
    PutChar(v);
    counter--;*/
    return c;
}
int main()
{
    char c = scan();
    PutChar(c);
    Halt();
    
}