#ifdef CHANGED
#include "copyright.h"
#include "system.h"
#include "userthread.h"
#include "synch.h"
#include "string"
//#define no_Threads 20

static Lock *lockAddrSpace = new Lock("Sem AddrSpace");
static Condition *CondSpace = new Condition("CondVar AddrSpace");
extern void StartProcess (char *filename);
const int pThread_id = 2000;

//int id_buffer[MAX_THREADS];
//int id_status[MAX_THREADS];
//std::string array[MAX_THREADS] = {"1001", "1002", "1003", "1004", "1005"};
const int Thread_id = 1000;

struct Func_args{
    int fun;
    int args;
};

static void StartUserThread(int f){ 
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
    machine->Run();
}

int do_UserThreadCreate(int f, int args) {
	int ret;
	//printf("Numer of clear bits in bit map = %d\n",currentThread->space->stackBitMap->NumClear());
	lockAddrSpace->Acquire ();
	if(currentThread->space->stackBitMap->NumClear() > 0 ){
		Func_args *fa = new Func_args;
		fa->fun = f;
		fa->args = args;
		//Thread_id++;
		int stackID = currentThread->space->stackBitMap->FindNew()+1;
		//int num = Thread_id + stackID;
		//char* name= new char[4];
		
		/*for(int i = 3; i>=0; i--, num/=10){
			name[i] = num%10 + '0';
			//printf("Characters = %c", num%10 + '0');
		}*/
		
		Thread *newThread = new Thread(currentThread->space->array[stackID -1].c_str());
		//printf("Thread create Thread[%d] = %s %s\n", Thread_id, name, newThread->getName() );
		//delete name;
	    
	
		currentThread->space->id_buffer[stackID-1] = Thread_id + stackID;    
		currentThread->space->id_status[stackID-1] = 0;
        lockAddrSpace->Release();
	
		//counter++;
		//stack_counter++;
		ret = Thread_id +stackID;
		
		newThread->Fork(StartUserThread, (int)fa);
        }
	else{
		fprintf(stderr, "Max thread count of %d reached. Can't create new thread \n", (int)MAX_THREADS);
		ret= -1;
	}
	
   return ret;
}

void do_UserThreadExit() {
	// counter--;
	char*name = (char*)currentThread->getName();
	std::string str(name);
	if(str.compare("main") && (name[0]-'0') != 2){
		
		//printf("\nHi from UserThreadExit = %s\n",currentThread->getName());
		int num = 1000;
		int check_id = 0;
	
		for(int i = 0 ; i < 4; i++, num/=10){
			check_id = num*(name[i] - '0')+ check_id;
			//printf("%d ", num*(name[i] - '0'));
		}
		//printf(" \nUser Thread Exit Thread[%d] = %s\n", check_id, name );
	    lockAddrSpace->Acquire ();
		for(int i = 0 ; i < MAX_THREADS ; i++){
			if(currentThread->space->id_buffer[i] == check_id){
				currentThread->space->id_status[i] = -1;
			}
		}
        currentThread->space->stackBitMap->Clear(check_id - Thread_id -1 );
        CondSpace->Signal(lockAddrSpace);
		lockAddrSpace->Release();
        
		currentThread->Finish();
	}
	
}


void do_UserThreadJoin(int id) {
	
	lockAddrSpace->Acquire ();
	printf("\nHi from UserThreadJoin = %s\n", currentThread->getName());
	
	for(int i = 0; i < MAX_THREADS  ; i++){
		if(currentThread->space->id_buffer[i] == id){
			while (currentThread->space->id_status[i] != -1){
				//printf("Thread[id] = %d is still not exited\n", currentThread->space->id_buffer[i]);
				//lockAddrSpace->Release();			//Conditional variable need to be impemented... Change busy waiting
				//currentThread->Yield();
				//lockAddrSpace->Acquire ();
                printf("Join id before sleeping = %d\n", id);
               CondSpace->Wait(lockAddrSpace);
			
			}
			currentThread->space->id_status[i] = 0;
		}
	}
	
	lockAddrSpace->Release();
}



static void StartUserProcess(int args){
	StartProcess((char*)args);
}

int do_userprocess_create(char *filename){
	int ret;
	printf("Numer of clear bits in bit map = \n");
    lockAddrSpace->Acquire ();
	if(ProcessID->NumClear() > 0 ){
		//int stackID = currentThread->space->stackBitMap->Find()+1;
        
		int stackID = ProcessID->FindNew() + pThread_id;
        
        int num = stackID;
        char* name= new char[5];
		
		for(int i = 3; i>=0; i--, num/=10){
			name[i] = num%10 + '0';
			//printf("Characters = %c", num%10 + '0');
		}		
		name[4] = '\0';
       //Thread *newThread = new Thread(parray[stackID - 1].c_str());
		Thread *newThread = new Thread(name);
        
		ret = stackID;
		
        currentThread->space->SaveState();
		lockAddrSpace->Release();
		
		newThread->Fork(StartUserProcess, (int)filename);
	}
	else{
         lockAddrSpace->Release();
		fprintf(stderr, "Max thread count of %d reached. Can't create new thread \n", (int)MAX_THREADS);
		ret= -1;
    }
	
   return ret;
}

#endif // CHANGED
