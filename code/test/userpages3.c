#include "syscall.h"
void print(void* ch)
{
    char* p = (char*)ch;
    PutChar(*p +1);
    PutChar(' ');
    
    UserThreadExit();
}

int main()
{	
	char c = GetChar();
    int i ;
    
    for(i = 0; i < 2; i++){
    
	   ForkExec("./userpages0a");
	   //PutChar('Z');
	   ForkExec("./userpages1a");
    }
    int Thread_id = UserThreadCreate(print, (void*)&c);
    PutInt(Thread_id);
    UserThreadJoin(Thread_id);
	//GetChar();
	PutChar('Q');
     //   Halt();
	return 0;
}
