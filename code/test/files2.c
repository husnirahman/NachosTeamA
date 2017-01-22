#include "syscall.h"

int main(){
	
	int n = fopen("small");
	PutInt(n);
	char s[5];
	fread("small",s,5);
	for (int i = 0; i<5 ; i++)
		PutChar(s[i]);
	return 0;
}
