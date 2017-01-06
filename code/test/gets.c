#include "syscall.h"


int main()
{
    char* s= Gets();
    Puts(s);
    Halt();
}