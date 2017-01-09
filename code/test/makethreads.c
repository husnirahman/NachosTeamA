#include "syscall.h"

void print(void* ch)
{
    char c = (char)((int)ch);
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
    PutChar((char)n);
    //print('a');
    char q = '0';
    while(q != 'q')
    	GetChar(q);

    Halt();
    return 0;
}