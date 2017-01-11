#ifdef CHANGED
#include "copyright.h"
#include "system.h"
#include "userthread.h"
#include "synch.h"
#include "string"
#define no_Threads 20

static Lock *lockAddrSpace = new Lock("Sem AddrSpace");

int id_buffer[no_Threads];
int id_status[no_Threads];
int Thread_id = 1000;
int counter = 0;
int stack_counter = 0;

struct Func_args{
    int fun;
    int args;
};

static void StartUserThread(int f){ 
	lockAddrSpace->Acquire ();
	//printf("Hi from Startuserthread = %s\n",currentThread->getName());
    Func_args *fa = (Func_args*)f;
    machine->WriteRegister (PCReg, fa->fun);
    machine->WriteRegister (NextPCReg, fa->fun + 4);
    machine->WriteRegister (StackReg, (machine->pageTableSize - 3*(stack_counter)) * PageSize - 16);
    //printf("Page table size = %d\n", machine->pageTableSize);
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
    Thread_id++;
    int num = Thread_id;
    char* name = new char[4];
    
    for(int i = 3; i>=0; i--, num/=10){
    	name[i] = num%10 + '0';
    	//printf("Characters = %c", num%10 + '0');
    }
    
    Thread *newThread = new Thread(name);
    //printf("Thread create Thread[%d] = %s %s\n", Thread_id, name, newThread->getName() );
    //delete name;
    
    id_buffer[counter] = Thread_id;
    id_status[counter] = 0;
    counter++;
    stack_counter++;
    lockAddrSpace->Release();
    newThread->Fork(StartUserThread, (int)fa);
    return Thread_id;
}

void do_UserThreadExit() {
	lockAddrSpace->Acquire ();
    counter--;
	const char*name = currentThread->getName();
	//printf("Hi from UserThreadExit = %s\n",currentThread->getName());
	int num = 1000;
	int check_id = 0;
	
	for(int i = 0 ; i < 4; i++, num/=10){
		check_id = num*(name[i] - '0')+ check_id;
		//printf("%d ", num*(name[i] - '0'));
	}
	//printf(" \nUser Thread Exit Thread[%d] = %s\n", check_id, name );
	
	for(int i = 0 ; i<no_Threads ; i++){
		if(id_buffer[i] == check_id){
			id_status[i] = -1;
		}
	}
	lockAddrSpace->Release();
    currentThread->Finish();
    
}

void do_UserThreadJoin(int id) {
	//printf("Hi from UserThreadJoin = %d\n",id);
	lockAddrSpace->Acquire ();
	
	for(int i = 0; i<no_Threads  ; i++){
		if(id_buffer[i] == id){
			while (id_status[i] != -1){
				//printf("Thread[id] = %d is still on exited\n", id_buffer[i]);
				lockAddrSpace->Release();
				currentThread->Yield();
				lockAddrSpace->Acquire ();
			
			}
		}
	}
	lockAddrSpace->Release();
}

#endif // CHANGED
