#ifdef CHANGED
#include "copyright.h"
#include "system.h"
#include "userthread.h"
#include "synch.h"
#include "string"
extern void StartProcess (char *filename);
//std::string parray[MAX_THREADS] = {"2001", "2002", "2003", "2004", "2005"};


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
    lockProcSpace->Acquire ();
	if(ProcessID->NumClear() > 0 ){
		//int stackID = currentThread->space->stackBitMap->Find()+1;
        
		int stackID = ProcessID->Find() + pThread_id;
        lockProcSpace->Release();
		
        int num = stackID;
        char* name= new char[5];
		
		for(int i = 3; i>=0; i--, num/=10){
			name[i] = num%10 + '0';
			//printf("Characters = %c", num%10 + '0');
		}		
		name[4] = '\0';
       //Thread *newThread = new Thread(parray[stackID - 1].c_str());
		Thread *newThread = new Thread(name);
        
        
		//pid_buffer[stackID-1] = pThread_id + stackID;    
		//pid_status[stackID-1] = 0;
        
        
		ret = stackID;
		
        currentThread->space->SaveState();
		
		newThread->Fork(StartUserProcess, (int)filename);
	}
	else{
         lockProcSpace->Release();
		fprintf(stderr, "Max thread count of %d reached. Can't create new thread \n", (int)MAX_THREADS);
		ret= -1;
    }
	
   return ret;
}
/*
void do_UserProcessExit() {
	char*name = (char*)currentThread->getName();
	std::string str(name);
	if(str.compare("main") || (name[0]-'0') == 2){
		
		//printf("Hi from UserProcessExit = %s\n",currentThread->getName());
		int check_id = (name[3] - '0') + pThread_id;
		//printf(" \nUser Thread Exit Thread[%d] = %s\n", check_id, name );
        
	    lockProcSpace->Acquire ();
		for(int i = 0 ; i < MAX_THREADS ; i++){
			if(pid_buffer[i] == check_id){
				pid_status[i] = -1;
			}
		}
		lockProcSpace->Release();
		
        currentThread->space->stackBitMap->Clear(check_id - pThread_id -1 );
		currentThread->Finish();
	}
}
*/

#endif//CHANGED

