#include "syscall.h"

void print(void* ch)
{
    char* p = (char*)ch;
    PutChar(*p);
    PutChar('\n');
    UserThreadExit();
}
void print2(void* ch)
{
    char* p = (char*)ch;
    PutChar(*p +1);
    PutChar('\n');
    UserThreadExit();
}

int main()
{
  //// void* p = c;
    char c = GetChar();
    //Puts("Enter Number of Threads below 5: ");
    int n = 1;
    int Thread_id[2*n];
    for(int i = 0; i<n; i++){
    	Thread_id[i] = (UserThreadCreate(print, (void*)&c));
    	PutChar('\n');
    	Thread_id[i+1] = (UserThreadCreate(print2, (void*)&c));
    	PutChar('\n');
    }
    Join(Thread_id[1]);
    Halt();
    return 0;
}
