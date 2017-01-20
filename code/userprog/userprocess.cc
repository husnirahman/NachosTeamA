#ifdef CHANGED
#include "copyright.h"
#include "system.h"
#include "userthread.h"
#include "synch.h"
#include "string"
extern void StartProcess (char *filename);
//std::string parray[MAX_THREADS] = {"2001", "2002", "2003", "2004", "2005"};


static Lock *lockProcSpace = new Lock("Sem Procspace");

const int pThread_id = 2000;

static void StartUserProcess(int args){
	StartProcess((char*)args);
}

int do_userprocess_create(char *filename){
	int ret;
	printf("Numer of clear bits in bit map = \n");
    lockProcSpace->Acquire ();
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
		lockProcSpace->Release();
		
		newThread->Fork(StartUserProcess, (int)filename);
	}
	else{
         lockProcSpace->Release();
		fprintf(stderr, "Max thread count of %d reached. Can't create new thread \n", (int)MAX_THREADS);
		ret= -1;
    }
	
   return ret;
}


#endif//CHANGED

