#include "syscall.h"


void thd2() {
    int fp = fopen("small");
    if(fp > -1) {
        fseek(fp,15);
        fwrite(fp,"main",3);
        fclose(fp);
    }
    UserThreadExit();
}

int main(){
	ForkExec("f4");
    int id2 = UserThreadCreate(thd2,(void*)0);
    
    UserThreadJoin(id2);
    int fp = fopen("small");
    if(fp > -1) {        
        fclose(fp);
    }
    return 0;
}
