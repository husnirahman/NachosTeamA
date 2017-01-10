#ifdef CHANGED
#include "copyright.h"
#include "system.h"
#include "userthread.h"

//static void ThreadCreate(int arg) { threadCreate->V(); }
//static void ThreadExit(int arg) { threadExit->V(); }

struct Func_args{
    int fun;
    int args;
};

//static Thread *newThread = Thread("new");
static void StartUserThread(int f){ 
   
    fprintf(stderr,"Hi from StartUserThread1\n");
    Func_args *fa = (Func_args*)f;
   
    machine->WriteRegister (PCReg, fa->fun);
    machine->WriteRegister (NextPCReg, fa->fun + 4);
    //int main_pointer = (int)machine->mainMemory;
    
    currentThread->space->RestoreState();
    
    //fprintf(stderr,"Hi from StartUserThread2\n");
    machine->WriteRegister (StackReg, (machine->pageTableSize - 3) * PageSize - 16);
    
    currentThread->space->RestoreState();
    machine->Run();
}

int do_UserThreadCreate(int f, int args) {
    Func_args *fa = new Func_args;
    fa->fun = f;
    fa->args = args;
    Thread *newThread = new Thread("new");
    //printf("Hi from do_userthread create\n");
    newThread->Fork(StartUserThread, (int)fa);
    return 0;
}

void do_UserThreadExit() {
    currentThread->Finish();
}

#endif // CHANGED
