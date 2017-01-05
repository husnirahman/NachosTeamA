#include "syscall.h"

char scan()
{
    char c = GetChar();
    return c;
}
int main()
{
    char c = scan();
    PutChar(c);
    Halt();
    
}