#include "syscall.h"


int main()
{
    char s[20] ;
    Gets(s, 20);
    Puts(s);
    PutChar('\n');
    Halt();
}