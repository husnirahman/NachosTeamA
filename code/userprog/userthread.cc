#ifdef CHANGED
#include "copyright.h"
#include "system.h"
#include "userthread.h"
#include "synch.h"

//static void ThreadCreate(int arg) { threadCreate->V(); }
//static void ThreadExit(int arg) { threadExit->V(); }
int counter = 0;
struct Func_args{
    int fun;
    int args;
};


//static Thread *newThread = Thread("new");
static void StartUserThread(int f){ 
    //fprintf(stderr,"Hi from StartUserThread1\n");
    //Func_args *fa = (Func_args*)f;
    Func_args *fa = (Func_args*)f;
    machine->WriteRegister (PCReg, fa->fun);
    machine->WriteRegister (NextPCReg, fa->fun + 4);
   
    //currentThread->space->RestoreState();
    
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
    //printf("Hi from do_userthread create %c\n", (char)fa->args);
    counter++;
    newThread->Fork(StartUserThread, (int)fa);
    return 0;
}

void do_UserThreadExit() {
    counter--;
    /*while(counter != 0){
       currentThread->Sleep();
    }*/
    
    currentThread->Finish();
    
    
}

void do_UserThreadJoin() {
    while(counter > 0) {
        currentThread->Yield();
    }
}

#endif // CHANGED
