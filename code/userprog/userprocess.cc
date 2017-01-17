#ifdef CHANGED
#include "copyright.h"
#include "system.h"
#include "userthread.h"
#include "synch.h"
#include "string"
extern void StartProcess (char *filename);
std::string parray[MAX_THREADS] = {"2001", "2002", "2003", "2004", "2005"};

static Lock *lockProcSpace = new Lock("Sem AddrSpace");

int pid_buffer[MAX_THREADS];
int pid_status[MAX_THREADS];
const int pThread_id = 2000;

static void StartUserProcess(int args){
	StartProcess((char*)args);
}

int do_userprocess_create(char *filename){
	int ret;
	//printf("Numer of clear bits in bit map = %d\n",currentThread->space->stackBitMap->NumClear());
	if(currentThread->space->stackBitMap->NumClear() > 0 ){
		int stackID = currentThread->space->stackBitMap->Find()+1;
		
		Thread *newThread = new Thread(parray[stackID - 1].c_str());
		
        lockProcSpace->Acquire ();
		pid_buffer[stackID-1] = pThread_id + stackID;    
		pid_status[stackID-1] = 0;
        lockProcSpace->Release();
        
		ret = pThread_id +stackID;
		
        currentThread->space->SaveState();
		
		newThread->Fork(StartUserProcess, (int)filename);
	}
	else{
		fprintf(stderr, "Max thread count of %d reached. Can't create new thread \n", (int)MAX_THREADS);
		ret= -1;
    }
	
   return ret;
}


#endif//CHANGED

