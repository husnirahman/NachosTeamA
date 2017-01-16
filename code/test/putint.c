#include "syscall.h"


int main()
{
    PutInt(2147483647);
    PutChar('\n');
    Halt();
    return 0;
}
