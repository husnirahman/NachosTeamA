// exception.cc 
//      Entry point into the Nachos kernel from user programs.
//      There are two kinds of things that can cause control to
//      transfer back to here from user code:
//
//      syscall -- The user code explicitly requests to call a procedure
//      in the Nachos kernel.  Right now, the only function we support is
//      "Halt".
//
//      exceptions -- The user code does something that the CPU can't handle.
//      For instance, accessing memory that doesn't exist, arithmetic errors,
//      etc.  
//
//      Interrupts (which can also cause control to transfer from user
//      code into the Nachos kernel) are handled elsewhere.
//
// For now, this only handles the Halt() system call.
// Everything else core dumps.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "system.h"
#include "syscall.h"
#include "synch.h"

#ifdef CHANGED
#include "sysdep.h"
#include "userthread.h"
#include "userprocess.h"
#include "string"
extern void StartProcess (char *filename);
static Lock *ProcessLock = new Lock("Process Lock");
extern void Exit(int status);
int proc_counter = 0;

#endif

//----------------------------------------------------------------------
// UpdatePC : Increments the Program Counter register in order to resume
// the user program immediately after the "syscall" instruction.
//----------------------------------------------------------------------
static void
UpdatePC ()
{
    int pc = machine->ReadRegister (PCReg);
    machine->WriteRegister (PrevPCReg, pc);
    pc = machine->ReadRegister (NextPCReg);
    machine->WriteRegister (PCReg, pc);
    pc += 4;
    machine->WriteRegister (NextPCReg, pc);
}


//----------------------------------------------------------------------
// ExceptionHandler
//      Entry point into the Nachos kernel.  Called when a user program
//      is executing, and either does a syscall, or generates an addressing
//      or arithmetic exception.
//
//      For system calls, the following is the calling convention:
//
//      system call code -- r2
//              arg1 -- r4
//              arg2 -- r5
//              arg3 -- r6
//              arg4 -- r7
//
//      The result of the system call, if any, must be put back into r2. 
//
// And don't forget to increment the pc before returning. (Or else you'll
// loop making the same system call forever!
//
//      "which" is the kind of exception.  The list of possible exceptions 
//      are in machine.h.
//----------------------------------------------------------------------

void
ExceptionHandler (ExceptionType which)
{
    int type = machine->ReadRegister (2);
    
    #ifndef CHANGED 
   
    if ((which == SyscallException) && (type == SC_Halt)) {
        DEBUG('a', "Shutdown, initiated by user program.\n");
        interrupt->Halt();
    } else {
        printf("Unexpected user mode exception %d %d\n", which, type);
        ASSERT(FALSE);
    }
    #else // CHANGED
    if (which == SyscallException) {
        switch (type) {
            case SC_Halt: {
                DEBUG('a', "Shutdown, initiated by user program.\n");
                interrupt->Halt();
            break;
            }
            case SC_PutChar: {
                int c = machine->ReadRegister (4);
                sc->SynchPutChar ((char)c);
                break;
            }
            case SC_Puts: {
                char *buffer = new char [MAX_STRING_SIZE];
                int from = machine->ReadRegister (4);
                copyStringFromMachine(from, buffer, MAX_STRING_SIZE);
                sc->SynchPutString (buffer);
                delete buffer;
                break;
            }
            case SC_GetChar: {
                char c = sc->SynchGetChar();
                if (c == EOF)
                    c = ' ';
                machine->WriteRegister (2, (int)c);
                break;
            }
            case SC_Gets: {
                int to = machine->ReadRegister(4);
                int size = machine->ReadRegister(5);
                char *buffer = new char[MAX_STRING_SIZE];
                sc->SynchGetString(buffer, size);
                copyStringToMachine(to, buffer, size);
                delete buffer;
                break;
            }
            case SC_PutInt: {
                int integer = machine->ReadRegister(4);
                sc->SynchPutInt(integer);
                break;
            }
            case SC_GetInt: {
                int integer;
                int address = machine->ReadRegister(4);
                sc->SynchGetInt(&integer);
                machine->WriteMem(address, 4, integer);
                break;
            }
            case SC_ThdCreate: { 
                int f = machine->ReadRegister(4);
                int args = machine->ReadRegister(5); 
                int n = do_UserThreadCreate(f, args);
                machine->WriteRegister(2, n);
                ASSERT(n != -1);
            	break;
            }
            case SC_ThdExit: {
                do_UserThreadExit();
            	break;
            }
            case SC_ThdJoin: {
            	int id = machine->ReadRegister(4);
            	do_UserThreadJoin(id);
                break;
            }
            case SC_ForkE: {
                ProcessLock->Acquire();
            	proc_counter++;
                ProcessLock->Release();
            	char *buffer = new char [MAX_STRING_SIZE];
                int file = machine->ReadRegister (4);
               	copyStringFromMachine(file, buffer, MAX_STRING_SIZE);
            	int n = do_userprocess_create(buffer);
            	printf("Process id = %d\n", n);
                machine->WriteRegister(2, n);
                break;
            }
            case SC_Exit: {
            	int status = machine->ReadRegister(4);
                ProcessLock->Acquire();
                if(status == 0){
                    if(proc_counter > 0){
                        printf("Process Counter = %d\n", proc_counter);
                        proc_counter--;
                        char*name = (char*)currentThread->getName();
                        std::string str(name);
                        if(str.compare("main")){
                            int check_id = (currentThread->getName()[3] - '0') + 2000;
                            //printf("process name = %s %d\n", currentThread->getName(), check_id);
                            //currentThread->space->stackBitMap->Clear(check_id - 2000 -1 );
                            ProcessID->Clear(check_id - 2000);
                        }
                        ProcessLock->Release();
                        currentThread->Finish();
                    }
            	   else{
                        ProcessLock->Release();
                        interrupt->Halt();
            	   }
                }
            	else{
            		fprintf(stderr, "Process cannot be exited\n");
                    ProcessLock->Release();
                }
                break;
            }
 #ifdef FILESYS
            case SC_createDir:{
            	char* buffer = new char[MAX_STRING_SIZE];
            	int file = machine->ReadRegister(4);
            	copyStringFromMachine(file, buffer, MAX_STRING_SIZE);
            	bool b = fileSystem->CreateD((const char*)buffer);
            	int n;
            	if(b) n = 0; else n =-1;
            	machine->WriteRegister(2,n);
            	delete buffer;
            	break;
            }
            case SC_changeDir:{
            	char* buffer = new char[MAX_STRING_SIZE];
            	int file = machine->ReadRegister(4);
            	copyStringFromMachine(file, buffer, MAX_STRING_SIZE);
            	bool b = fileSystem->ChangeD((const char*)buffer);
            	int n;
            	if(b) n = 0; else n =-1;
            	machine->WriteRegister(2,n);
            	delete buffer;
            	break;
            }
            case SC_displayDir:{
            	fileSystem->List();
            	break;
            }
            case SC_fopen:{
            	char* buffer = new char[MAX_STRING_SIZE];
            	int file = machine->ReadRegister(4);
            	copyStringFromMachine(file, buffer, MAX_STRING_SIZE);
            	int n = fileSystem->fileopen((const char*)buffer);
            	machine->WriteRegister(2,n);
                delete buffer;
            	break;
            }
            case SC_fread:{
            	int file = machine->ReadRegister(4);            	            	
            	int size = machine->ReadRegister(6);
                char* to = new char[size];
            	int r = fileSystem->fileread(file, to, size);
            	if(r == 0){
                    int dest = machine->ReadRegister(5);
                    bool b = TRUE;
                    int i = 0;
                    while(i < size && b == TRUE){
    				    b = machine->WriteMem(dest + i, 1, (int)(*(to + i)));
        			     i++;
                    }
                }                
            	machine->WriteRegister(2,r);
                delete to;
                break;
            	
            }
            
            case SC_fwrite:{
            	int file = machine->ReadRegister(4);            	
            	int dest = machine->ReadRegister(5);
                int size = machine->ReadRegister(6);
            	char* from = new char[size];
            	copyStringFromMachine(dest, from, size);            	
            	int r = fileSystem->filewrite(file, from, size);
            	machine->WriteRegister(2,r);
                
                delete from;
                break;
            	
            }
            case SC_fclose:{
                int file = machine->ReadRegister(4);                
                int r = fileSystem->fileclose(file);
            	machine->WriteRegister(2,r);
            	break;
            }
            case SC_fseek:{
                int file = machine->ReadRegister(4);                
                int position = machine->ReadRegister(5);                
                int r = fileSystem->fileseek(file,position);                
            	machine->WriteRegister(2,r);
            	break;
            }
            case SC_fcreate:{
                char* buffer = new char[MAX_STRING_SIZE];
            	int file = machine->ReadRegister(4);
                int size = machine->ReadRegister(5);
            	copyStringFromMachine(file, buffer, MAX_STRING_SIZE);
            	bool b = fileSystem->Create((const char*)buffer, size);
                int n;
                if (b) n =0; else n = -1;
            	machine->WriteRegister(2,n);
                break;
            }
#endif //FILESYS
            default: {
                printf("Unexpected user mode exception %d %d\n", which, type);
                ASSERT(FALSE);
            }
        }
    }    
    #endif // CHANGED
    
    // LB: Do not forget to increment the pc before returning!
    //printf("hi from exc %d %d %d \n", which, type, machine->ReadRegister(31));    
    UpdatePC();
    // End of addition
}

