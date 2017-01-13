#include "syscall.h"

void print(void* ch)
{
	//UserThreadJoin(1003);
    char* p = (char*)ch;
    PutChar(*p);
    PutChar(' ');
    PutChar('\n');
    Puts("hello\n");
    UserThreadExit();
}
void print2(void* ch)
{
    char* p = (char*)ch;
    PutChar(*p +1);
    PutChar(' ');
    PutChar('\n');
    UserThreadExit();
}

int main()
{
 	char c = GetChar();
    int n = 1;
    int Thread_id[2*n +1];
    int i;
    for(i = 0; i<2*n; i+=2){
    	Thread_id[i] = UserThreadCreate(print, (void*)&c);
    	Thread_id[i+1] = UserThreadCreate(print, (void*)&c);
    	}
    
    Thread_id[2] = UserThreadCreate(print2, (void*)&c);
    
    for( i = 0 ; i<2*n ; i++)
    	UserThreadJoin(Thread_id[i]);
    PutInt(Thread_id[0]);
    	
    Puts("Bye\n");
    Halt();
    return 0;
}
