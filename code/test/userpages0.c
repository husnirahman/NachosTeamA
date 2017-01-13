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
{	//int i;
	int id =UserThreadCreate(f, (void *) THIS);
	f((void*) THAT);
	
	UserThreadJoin(id);
	return 0;
}
