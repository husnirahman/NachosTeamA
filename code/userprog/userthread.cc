#ifdef CHANGED
#include "copyright.h"
#include "system.h"
#include "userthread.h"
#include "synch.h"
#include "string"

static Lock *lockAddrSpace = new Lock("Sem AddrSpace");
int Thread_id = 10;
int counter = 0;
struct Func_args{
    int fun;
    int args;
};

static void StartUserThread(int f){ 
	lockAddrSpace->Acquire ();
    Func_args *fa = (Func_args*)f;
    machine->WriteRegister (PCReg, fa->fun);
    machine->WriteRegister (NextPCReg, fa->fun + 4);
    machine->WriteRegister (StackReg, (machine->pageTableSize - 3) * PageSize - 16);
    machine->WriteRegister(4, fa->args);
    currentThread->space->RestoreState();
    lockAddrSpace->Release();
    machine->Run();
}

int do_UserThreadCreate(int f, int args) {
	lockAddrSpace->Acquire ();
    Func_args *fa = new Func_args;
    fa->fun = f;
    fa->args = args;
    Thread *newThread = new Thread("new");
    counter++;
    Thread_id++;
    lockAddrSpace->Release();
    newThread->Fork(StartUserThread, (int)fa);
    return Thread_id;
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
