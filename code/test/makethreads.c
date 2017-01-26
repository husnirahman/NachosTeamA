#include "syscall.h"

void print(void* ch)
{
	//UserThreadJoin(1003);
    char* p = (char*)ch;
    PutChar(*p);
    PutChar(' ');
    Puts("hello");
   
    UserThreadExit();
}
void print2(void* ch)
{
    char* p = (char*)ch;
    PutChar(*p +1);
     int id = UserThreadCreate(print, ch);
    UserThreadJoin(id);
    
    UserThreadExit();
}

int main()
{
 	char c = 'K';
    int n = 1;
    int Thread_id[2*n +1];
    int i = 0;
    //for(i = 0; i<2*n; i+=2){
    	Thread_id[i] = UserThreadCreate(print2, (void*)&c);
    	Thread_id[i+1] = UserThreadCreate(print2, (void*)&c);
    	//}
    
    //Thread_id[2] = UserThreadCreate(print2, (void*)&c);
    
    for( i = 0 ; i<1 ; i++)
    	UserThreadJoin(Thread_id[i]);
   PutInt(Thread_id[0]);
    	
    	
    Puts("Bye");
    Halt();
    return 0;
}
