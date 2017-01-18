#include "syscall.h"


int main()
{
    char s[100] ;
    Gets(s, 8);
    Puts(s);
    Halt();
}