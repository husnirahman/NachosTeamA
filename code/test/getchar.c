#include "syscall.h"
char scan()
{
    char c = GetChar();
    PutChar(c);
    PutChar('\n');
    
    
    char v = GetChar();
    PutChar(v);
    PutChar('\n');

    char s = GetChar();
    PutChar(s);
    PutChar('\n');

    PutChar(v);
    PutChar('\n');
    return c;
}
int main()
{
    char c = scan();
    PutChar(c);
    PutChar('\n');
    Halt();
    
}
