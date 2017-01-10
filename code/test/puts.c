#include "syscall.h"


int main()
{
    char c[] = "abc\0";
    Puts(c);
    PutChar('a');
    Halt();
}
