#include "syscall.h"


void thd2() {
    int n = fopen("small");
    if(n==0) {
        //fseek("small",0);
        fwrite("small","main",15,3);
        fclose("small");
    }
    UserThreadExit();
}

int main(){
	ForkExec("f4");
    int id2 = UserThreadCreate(thd2,(void*)0);
    
    UserThreadJoin(id2);
    int n = fopen("small");
    if(n==0) {        
        fclose("small");
    }
    return 0;
}