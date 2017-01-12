#ifdef CHANGED
#include "copyright.h"
#include "system.h"
#include "userthread.h"
#include "synch.h"

static Lock *thdLock = new Lock("ThreadLock");
int thdID = 1000;
int stack_counter = 0;

struct Func_args{
    int fun;
    int args;
};

static void StartUserThread(int f){ 
	thdLock->Acquire ();
	
	    Func_args *fa = (Func_args*)f;
	    machine->WriteRegister (PCReg, fa->fun);
	    machine->WriteRegister (NextPCReg, fa->fun + 4);
	    machine->WriteRegister (StackReg, (machine->pageTableSize - 3*(stack_counter)) * PageSize - 16);
	    machine->WriteRegister(4, fa->args);
	    currentThread->space->RestoreState();

    thdLock->Release();
    machine->Run();
}

int do_UserThreadCreate(int f, int args) {
	if(thdList->getCount() < MAX_THREAD) {

		thdLock->Acquire ();

		    Func_args *fa = new Func_args;
		    fa->fun = f;
		    fa->args = args;
		        
		    Thread *newThread = new Thread("new");
		    newThread->setID(thdID);
		    Node* newNode = new Node(thdID);
		    newNode->setNext(NULL);
		    newNode->setStatus(LIVE);		
		    thdList->addNode(newNode);

		    stack_counter++;
		    thdID++;

		thdLock->Release();
		newThread->Fork(StartUserThread, (int)fa);

	    return thdID;
	} else {
		fprintf(stderr, "Maximum thread count reached. Can't create new thread.\n");
		return -1;
	}	
}

void do_UserThreadExit() {
	thdLock->Acquire ();	

		int id = currentThread->getID();
		Node* node = thdList->searchNode(id);
		node->setStatus(DEAD);

	thdLock->Release();
    currentThread->Finish();    
}

void do_UserThreadJoin(int id) {
	thdLock->Acquire ();

	Node* node = thdList->searchNode(id);
	if(node == NULL) {
		fprintf(stderr, "The thread to be joined doesn't exist.\n");		
	} else {
		while(node->getStatus() == LIVE) {
				thdLock->Release();
					currentThread->Yield();
				thdLock->Acquire ();
		}
		thdList->removeNode(node);
	}
	thdLock->Release();
}

#endif // CHANGED
