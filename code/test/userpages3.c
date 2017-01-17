#include "syscall.h"
/*void print(void* ch)
{
    char* p = (char*)ch;
    PutChar(*p +1);
    PutChar(' ');
    
    UserThreadExit();
}
*/
int main()
{	
	//char c = GetChar();
    
	ForkExec("./userpages0a");
	//PutChar('Z');
	ForkExec("./userpages1a");
       
    //int Thread_id =  UserThreadCreate(print, (void*)&c);
    //UserThreadJoin(Thread_id);
	//GetChar();
	PutChar('Q');
     //   Halt();
	return 0;
}
