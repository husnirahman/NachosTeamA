#include "syscall.h"
#define THIS "aaa"
#define THAT "bbb"

const int N = 1; // Choose it large enough!
/*
void putst(char *s)
{
	char *p; for (p = s; *p != '\0'; p++) PutChar(*p);
}

void f(void *s)
{
	int i; for (i = 0; i < N; i++) putst((char *)s);
	UserThreadExit();
}*/

int main()
{	
	PutChar('A');
        /*
	//int n = 2;
	int id;
	//int i;
        
	//for(i = 0 ; i <n ; i ++)
		id = UserThreadCreate(f, (void *) THIS);
	
	f((void*) THAT);
	
	for(i = 0 ; i <n ; i ++)
		UserThreadJoin(id[i]);
	
	int id3 = UserThreadCreate(f, (void *) THIS);
	UserThreadJoin(id);
	//PutInt(id3);*/
	return 0;
}
