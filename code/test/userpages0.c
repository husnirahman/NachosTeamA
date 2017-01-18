#include "syscall.h"
#define THIS "a"
#define THAT "bbb"

const int N = 1; // Choose it large enough!

void putst(char *s)
{
	char *p; for (p = s; *p != '\0'; p++) PutChar(*p);
}
void f2(void *s)
{
	int i; for (i = 0; i < N; i++) putst((char *)s);
	//UserThreadJoin(1002);
	UserThreadExit();
}

void f(void *s)
{
	int i; for (i = 0; i < N; i++) putst((char *)s);
	//UserThreadJoin(1002);
	UserThreadExit();
}

int main()
{	
	int n = 2000;
	int id1,id2,id3,id4,id5;
	int i;
        
	for(i = 0 ; i <n ; i ++){
		id1 = UserThreadCreate(f, (void *) THIS);
		id2 = UserThreadCreate(f, (void *) THAT);
		id3 = UserThreadCreate(f, (void *) THIS);
		id4 = UserThreadCreate(f, (void *) THAT);
		id5 = UserThreadCreate(f, (void *) THAT);
		
		UserThreadJoin(id1);
		UserThreadJoin(id2);
		UserThreadJoin(id3);
		UserThreadJoin(id4);
		UserThreadJoin(id5);
	}
	
	//f((void*) THAT);
	
	//for(i = 0 ; i <n ; i ++)
		
	
	//int id3 = UserThreadCreate(f, (void *) THIS);
	//UserThreadJoin(id3);
	//PutInt(id3);
	
	return 0;
}
