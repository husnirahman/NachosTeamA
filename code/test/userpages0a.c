#include "syscall.h"
#define THIS "aaa"
#define THAT "bbb"

const int N = 1; // Choose it large enough!

void putst(char *s)
{
	char *p; for (p = s; *p != '\0'; p++) PutChar(*p);
}

void f(void *s)
{
	int i; for (i = 0; i < N; i++) putst((char *)s);
	UserThreadExit();
}

int main()
{	
	int n = 2;
	int id[n];
	
	for(int i = 0 ; i <n ; i ++)
		id[i] = UserThreadCreate(f, (void *) THIS);
	
	f((void*) THAT);
	
	for(int i = 0 ; i <n ; i ++)
		UserThreadJoin(id[i]);
	
	int id3 = UserThreadCreate(f, (void *) THIS);
	UserThreadJoin(id3);
	//PutInt(id3);
	
	//Exit(0);
	//Halt();
	//while(1){;}
	return 0;
}
