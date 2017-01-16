#include "syscall.h"
#define THIS "aaa"
#define THAT "bbb"

const int N = 1; // Choose it large enough!

void putst(char *s)
{
	char *p; for (p = s; *p != '\0'; p++) PutChar(*p);	
	Puts("\nEnd PutS\n");
}

void f(void *s)
{
	int i; for (i = 0; i < N; i++) putst((char *)s);	
	Puts("\nEnd\n");
	UserThreadExit();
}

int main()
{	
	int id = UserThreadCreate(f, (void *) THIS);	
	PutInt(id);	
	f((void*) THAT);
	PutInt(id);
	UserThreadJoin(id);
	Halt();
	return 0;
}
