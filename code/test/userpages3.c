#include "syscall.h"

int main()
{
	ForkExec("./userpages0");
	ForkExec("./userpages1");
	Halt();
	return 0;
}
