#ifdef CHANGED
#include "copyright.h"
#include "system.h"
#include "userthread.h"
#include "synch.h"
#include <string>

static Lock *thdLock = new Lock("ThreadLock");

struct Func_args{
    int fun;
    int args;
};

static void StartUserThread(int f){ 
	thdLock->Acquire ();
		printf("hi from User StartUserThread create \n");
	    Func_args *fa = (Func_args*)f;
	    machine->WriteRegister (PCReg, fa->fun);
	    machine->WriteRegister (NextPCReg, fa->fun + 4);
	    
	    machine->WriteRegister (StackReg, (machine->pageTableSize - 3*(currentThread->getID())) * PageSize - 16);
	    machine->WriteRegister(4, fa->args);
	    currentThread->space->RestoreState();
		//printf("hi from User StartUserThread create 2\n");
    thdLock->Release();
    machine->Run();
}

int do_UserThreadCreate(int f, int args) {
	if(thdList->getCount() < MAX_THREAD) {

		thdLock->Acquire ();

		    Func_args *fa = new Func_args;
		    fa->fun = f;
		    fa->args = args;    
	    
	    	int thdID = currentThread->space->stackBitMap->Find();
	    	printf("hi from User thread create thd id : %d\n",thdID);		
	    	
		    Thread *newThread = new Thread("new");		    
	    	newThread->setID(thdID);
		    Node* newNode = new Node(thdID);
		    newNode->setNext(NULL);
		    newNode->setStatus(LIVE);		
		    thdList->addNode(newNode);   
		    
			//printf("hi from User thread create 2 \n");
		thdLock->Release();
		newThread->Fork(StartUserThread, (int)fa);
		//printf("hi from User thread create 3 \n");
	    return thdID;
	} else {
		fprintf(stderr, "Maximum thread count reached. Can't create new thread.\n");
		return -1;
	}

	
}

void do_UserThreadExit() {
	char* s = (char*)currentThread->getName();
	std::string str(s);
	printf("thread on exit = %s\n", s);
	if(str.compare("main")) {
	
	printf("thread on exit enter = %s\n", s);
		thdLock->Acquire ();
			
			printf("hi from thread exit\n");
			int id = currentThread->getID();
			Node* node = thdList->searchNode(id);
			node->setStatus(DEAD);

		thdLock->Release();
		currentThread->Finish();
		currentThread->space->stackBitMap->Clear(id);
    }
}

void do_UserThreadJoin(int id) {
	thdLock->Acquire ();
	Node* node = thdList->searchNode(id);
	printf("hi from thread join thd id:%d\n",id);
	if(node == NULL) {
		fprintf(stderr, "The thread to be joined doesn't exist.\n");		
	} else {
		while(node->getStatus() == LIVE) {
				thdLock->Release();
				currentThread->Yield();
				node = thdList->searchNode(id);
				thdLock->Acquire ();
		}
		thdList->removeNode(node);
	}

	thdLock->Release();
}

#endif // CHANGED
