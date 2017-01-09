#include "syscall.h"

void print(char c)
{
    int i;
    for (i = 0; i < 5; i++) {
        PutChar(c+i);
    }
    PutChar('\n');
}
int main()
{
    //do_UserThreadCreate(int f, int arg);
    char c = 'a';
    int n = UserThreadCreate(print((void*)c), (void*)c);
    //print('a');
    Halt();
    return 0;
}