#include "syscall.h"


int main()
{
    char s[5] ;
    Gets(s, 5);
    Puts(s);
    Halt();
}