#ifdef CHANGED
#include "copyright.h"
#include "system.h"
#include "userthread.h"

class Func_args{
public:
    Thread *thr;
    int fun;
    int args;
};

//static Thread *newThread = Thread("new");
static void StartUserThread(int f){
    Func_args* fn = new (Func_args); 
    fn = (Func_args*)f;
    Thread *newThread = fn->thr;
    int func = fn->fun;
    int args = fn ->args;
    newThread->Fork((void)(*func)(args),args);
}

int do_UserThreadCreate(int f, int args) {
    
    Func_args *fn = new (Func_args);
    fn->thr = new Thread("new");
    fn->fun    = f; 
    fn->args = args;
	StartUserThread((int)fn);
	return 0;
}

void do_UserThreadExit() {

}

#endif // CHANGED
