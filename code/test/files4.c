#include "syscall.h"

void thd1() {
    int fp = fopen("small");
    if(fp > -1) {        
        fseek(fp,10);
        fwrite(fp,"Hello", 5);
        fclose(fp);
    }
    UserThreadExit();
}

void thd2() {
    int fp = fopen("small");
    if(fp > -1) {
        fseek(fp,0);
        fwrite(fp,"End",3);
        fclose(fp);
    }
    UserThreadExit();
}

int main(){
	int id1 =  UserThreadCreate(thd1,(void*)0);
    int id2 = UserThreadCreate(thd2,(void*)0);
    
    UserThreadJoin(id1);
    UserThreadJoin(id2);
    
    int fp = fopen("small");
    if(fp > -1) {        
        fclose(fp);
    }
    return 0;
}
