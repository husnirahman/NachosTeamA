#ifdef CHANGED
#include "copyright.h"
#include "system.h"
#include "userthread.h"
#include "synch.h"
#include "string"
extern void StartProcess (char *filename);

struct Proc_args{
    char * s;
};

static Lock *lockProcSpace = new Lock("Sem AddrSpace");

int pid_buffer[MAX_THREADS];
int pid_status[MAX_THREADS];
const int pThread_id = 2000;

static void StartUserProcess(int args){
	
	//printf("hi from StartProcess %s \n",(char*)args);
	StartProcess((char*)args);
}

int do_userprocess_create(char *filename){
	lockProcSpace->Acquire ();
	int ret;
	//printf("Numer of clear bits in bit map = %d\n",currentThread->space->stackBitMap->NumClear());
	if(currentThread->space->stackBitMap->NumClear() > 0 ){
		int stackID = currentThread->space->stackBitMap->Find()+1;
		int num = pThread_id + stackID;
		char* name= new char[4];
		
		for(int i = 3; i>=0; i--, num/=10){
			name[i] = num%10 + '0';
			//printf("Characters = %c", num%10 + '0');
		}
		
		Thread *newThread = new Thread(name);
		
		pid_buffer[stackID-1] = pThread_id + stackID;    
		pid_status[stackID-1] = 0;
		ret = pThread_id +stackID;
		
		lockProcSpace->Release();
		newThread->Fork(StartUserProcess, (int)filename);
	}
	else{
		fprintf(stderr, "Max thread count of %d reached. Can't create new thread \n", (int)MAX_THREADS);
		lockProcSpace->Release();
		ret= -1;
	}
	
   return ret;
}


#endif//CHANGED

