#include "syscall.h"

void print(void* ch)
{
    char c = (char)ch;
    int i;
    for (i = 0; i < 5; i++) {
        PutChar(c+i);
    }
    PutChar('\n');
}
int main()
{
    //do_UserThreadCreate(int f, int arg);
    void* c = (void*)'a';
    int n = UserThreadCreate(print, c);
    //print('a');
    Halt();
    return 0;
}