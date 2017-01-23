#include "syscall.h"

void thd1() {
    int n = fopen("small");
    if(n==0) {        
        //fseek("small",10);
        fwrite("small","Hello",10, 5);
        fclose("small");
    }
    UserThreadExit();
}

void thd2() {
    int n = fopen("small");
    if(n==0) {
        //fseek("small",0);
        fwrite("small","End",0,3);
        fclose("small");
    }
    UserThreadExit();
}

int main(){
	int id1 =  UserThreadCreate(thd1,(void*)0);
    int id2 = UserThreadCreate(thd2,(void*)0);
    
    UserThreadJoin(id1);
    UserThreadJoin(id2);
    
    int n = fopen("small");
    if(n==0) {        
        fclose("small");
    }
    return 0;
}