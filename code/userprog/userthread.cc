#ifdef CHANGED
#include "copyright.h"
#include "system.h"
#include "userthread.h"
#include "synch.h"


int counter = 0;
struct Func_args{
    int fun;
    int args;
};

static void StartUserThread(int f){ 
    Func_args *fa = (Func_args*)f;
    machine->WriteRegister (PCReg, fa->fun);
    machine->WriteRegister (NextPCReg, fa->fun + 4);
    machine->WriteRegister (StackReg, (machine->pageTableSize - 3) * PageSize - 16);
    machine->WriteRegister(4, fa->args);
    currentThread->space->RestoreState();
    machine->Run();
}

int do_UserThreadCreate(int f, int args) {
    Func_args *fa = new Func_args;
    fa->fun = f;
    fa->args = args;
    Thread *newThread = new Thread("new");
    counter++;
    newThread->Fork(StartUserThread, (int)fa);
    return 35;
}

void do_UserThreadExit() {
    counter--;
    currentThread->Finish();
}

void do_UserThreadJoin() {
    while(counter > 0) {
        currentThread->Yield();
    }
}

#endif // CHANGED
