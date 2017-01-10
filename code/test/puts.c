#include "syscall.h"


int main()
{
    char c[] = "abc\n";
    Puts(c);
    PutChar('a');
    Halt();
}
