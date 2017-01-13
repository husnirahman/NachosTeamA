#ifdef CHANGED
#include "copyright.h"
#include "system.h"
#include "userthread.h"
#include "synch.h"
#include "string"
//#define no_Threads 20

static Lock *lockAddrSpace = new Lock("Sem AddrSpace");

int id_buffer[MAX_THREADS];
int id_status[MAX_THREADS];
const int Thread_id = 1000;
//int counter = 0;

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
    
    char*name = (char*)currentThread->getName();
    int num = Thread_id;
	int check_id = 0;

	for(int i = 0 ; i < 4; i++, num/=10){
		check_id = num*(name[i] - '0')+ check_id;
		//printf("%d ", num*(name[i] - '0'));
	}
	
    machine->WriteRegister (StackReg, (machine->pageTableSize - 3*(check_id -Thread_id)) * PageSize - 16); // need to reset a stack counter..., user bitmap...
    //printf("Page table size = %d\n", machine->pageTableSize);
    machine->WriteRegister(4, fa->args);
    currentThread->space->RestoreState();
    lockAddrSpace->Release();
    machine->Run();
}

int do_UserThreadCreate(int f, int args) {
	lockAddrSpace->Acquire ();
	int ret;
	//printf("Numer of clear bits in bit map = %d\n",currentThread->space->stackBitMap->NumClear());
	if(currentThread->space->stackBitMap->NumClear() > 0 ){
		Func_args *fa = new Func_args;
		fa->fun = f;
		fa->args = args;
		//Thread_id++;
		int stackID = currentThread->space->stackBitMap->Find()+1;
		int num = Thread_id + stackID;
		char* name= new char[4];
		
		for(int i = 3; i>=0; i--, num/=10){
			name[i] = num%10 + '0';
			//printf("Characters = %c", num%10 + '0');
		}
		
		Thread *newThread = new Thread(name);
		//printf("Thread create Thread[%d] = %s %s\n", Thread_id, name, newThread->getName() );
		//delete name;
	
		id_buffer[stackID-1] = Thread_id + stackID;    
		id_status[stackID-1] = 0;
		//counter++;
		//stack_counter++;
		ret = Thread_id +stackID;
		lockAddrSpace->Release();
		newThread->Fork(StartUserThread, (int)fa);
	}
	else{
		fprintf(stderr, "Max thread count of %d reached. Can't create new thread \n", (int)MAX_THREADS);
		lockAddrSpace->Release();
		ret= -1;
	}
	
   return ret;
}

void do_UserThreadExit() {
	lockAddrSpace->Acquire ();
   // counter--;
	char*name = (char*)currentThread->getName();
	std::string str(name);
	if(str.compare("main")){
	
	
		//printf("Hi from UserThreadExit = %s\n",currentThread->getName());
		int num = 1000;
		int check_id = 0;
	
		for(int i = 0 ; i < 4; i++, num/=10){
			check_id = num*(name[i] - '0')+ check_id;
			//printf("%d ", num*(name[i] - '0'));
		}
		//printf(" \nUser Thread Exit Thread[%d] = %s\n", check_id, name );
	
		for(int i = 0 ; i < MAX_THREADS ; i++){
			if(id_buffer[i] == check_id){
				id_status[i] = -1;
			}
		}
		currentThread->space->stackBitMap->Clear(check_id - Thread_id -1 );
		lockAddrSpace->Release();
    	currentThread->Finish();
	}
	
	lockAddrSpace->Release();
    //currentThread->Finish();
    
}

void do_UserThreadJoin(int id) {
	
	lockAddrSpace->Acquire ();
	//printf("Hi from UserThreadJoin = %d\n",id);
	
	for(int i = 0; i < MAX_THREADS  ; i++){
		if(id_buffer[i] == id){
			while (id_status[i] != -1){
				//printf("Thread[id] = %d is still on exited\n", id_buffer[i]);
				lockAddrSpace->Release();			//Conditional variable need to be impemented... Change busy waiting
				currentThread->Yield();
				lockAddrSpace->Acquire ();
			
			}
		}
	}
	lockAddrSpace->Release();
}

#endif // CHANGED
