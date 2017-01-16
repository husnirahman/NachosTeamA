#ifdef CHANGED
#include "copyright.h"
#include "system.h"
#include "userthread.h"
#include "synch.h"

#include <string>

static Lock *thdLock = new Lock("ThreadLock");
static Condition* joinCond = new Condition("JoinCOndition");

struct Func_args{
    int fun;
    int args;
};

static void StartUserThread(int f){ 
	
	    	printf("WTF : %s\n",currentThread->getName());		
	thdLock->Acquire ();
	
	    	printf("OH : %s\n",currentThread->getName());		
	    Func_args *fa = (Func_args*)f;
	    machine->WriteRegister (PCReg, fa->fun);
	    machine->WriteRegister (NextPCReg, fa->fun + 4);
	    
	    machine->WriteRegister (StackReg, (machine->pageTableSize - 4*(currentThread->getID())) * PageSize - 16);
	    machine->WriteRegister(4, fa->args);
	    
    thdLock->Release();
    machine->Run();
}

int do_UserThreadCreate(int f, int args) {
	
	BitMap* bitmap = currentThread->space->stackBitMap;
	
	if(bitmap->NumClear() > 0) {

		thdLock->Acquire ();

		    Func_args *fa = new Func_args;
		    fa->fun = f;
		    fa->args = args;    
	    
	    	int thdID = bitmap->Find();
	    	//printf("hi from User thread create thd id : %d\n",thdID);		
	    	
		  Thread *newThread = new Thread("new");		    
	    newThread->setID(thdID);
			thdLock->Release();
			newThread->Fork(StartUserThread, (int)fa);
	    return thdID;
	    
	} else {
		fprintf(stderr, "Maximum thread count reached. Can't create new thread.\n");
		return -1;
	}	
}

void do_UserThreadExit() {

	char* s = (char*)currentThread->getName();
	std::string str(s);
	
	printf("Exit from %s\n",s);
	if(str.compare("main")) {	
		thdLock->Acquire ();
		
		currentThread->space->stackBitMap->Clear(currentThread->getID());
		joinCond->Signal(thdLock);
		
		thdLock->Release();
		
		currentThread->Finish();
  } 
}

void do_UserThreadJoin(int id) {
	BitMap* bitmap = currentThread->space->stackBitMap;
	printf("hi from thread join thd id:%d\n",id);
	char* s = (char*)currentThread->getName();
	printf("Join from %s\n",s);
	
	thdLock->Acquire ();
	
	printf("OH LALA:%d\n",id);
	if(!bitmap->Test(id)) {
		fprintf(stderr, "The thread to be joined doesn't exist.\n");		
	} else {
		while(bitmap->Test(id)) {
		
	thdLock->Release();
	
    IntStatus oldLevel = interrupt->SetLevel (IntOff);
			currentThread->Sleep();	
    (void) interrupt->SetLevel (oldLevel);		
	thdLock->Acquire();
		}
	}
	thdLock->Release();
}

#endif // CHANGED
