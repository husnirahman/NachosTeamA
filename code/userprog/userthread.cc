#ifdef CHANGED
#include "copyright.h"
#include "system.h"
#include "userthread.h"


//static void ThreadCreate(int arg) { threadCreate->V(); }
//static void ThreadExit(int arg) { threadExit->V(); }

static int counter = 0;
struct Func_args{
    int fun;
    int args;
};


//static Thread *newThread = Thread("new");
static void StartUserThread(int f){ 
   
     printf("Hi from StartUserThread1\n");
    Func_args *fa = (Func_args*)f;
   
    machine->WriteRegister (PCReg, fa->fun);
    machine->WriteRegister (NextPCReg, fa->fun + 4);
    int main_pointer = (int)machine->mainMemory;
    printf("Hi from StartUserThread2\n");
    machine->WriteRegister (StackReg, main_pointer - 3 * PageSize - 16);
    printf("Hi from StartUserThread3 %d\n",main_pointer - 3 * PageSize - 16 );
    
    machine->pageTable = pageTable;
    machine->pageTableSize = numPages;
    
    machine->Run();
    printf("Hi from StartUserThread4\n");

    counter--;
}

int do_UserThreadCreate(int f, int args) {
    Func_args *fa = new Func_args;
    fa->fun = f;
    fa->args = args;
    Thread *newThread = new Thread("new");
    printf("Hi from do_userthread create\n");
    newThread->Fork(StartUserThread, (int)fa);
    counter++;
    return 0;
}

void do_UserThreadExit() {
    while (counter > 0){
        currentThread->Yield();
    }
    currentThread->Finish();
    
}

#endif // CHANGED
