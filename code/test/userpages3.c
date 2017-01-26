#include "syscall.h"
void print(void* ch)
{
    char* p = (char*)ch;
    PutChar(*p +1);
    //PutChar(' ');
    
    UserThreadExit();
}

int main()
{	
	char c = 'T';
    int i ;
    int n = 3;
    for(i = 0; i < n; i++){
    
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
