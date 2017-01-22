#include "syscall.h"

int main(){
	
	int n = fopen("small");
	PutInt(n);
	n = fopen("big");
	PutInt(n);
	
	return 0;
}
