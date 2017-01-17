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
extern void StartProcess (char *filename);
//static Lock *ProcessLock = new Lock("Process Lock");
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
                //printf("hi from putchar\n");
                int c = machine->ReadRegister (4);
                //printf("putchar from exception from thread = %c %s\n", c, currentThread->getName());
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
                //printf("getchar from exception from string  = %c %s\n", c, currentThread->getName());
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
                //printf("hi from putint\n");
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
            	char *buffer = new char [MAX_STRING_SIZE];
                int file = machine->ReadRegister (4);
               	copyStringFromMachine(file, buffer, MAX_STRING_SIZE);
            	printf("hi from ForkE %s\n", buffer);
            	int n = do_userprocess_create(buffer);
            	printf("Process id = %d\n", n);
            	proc_counter++;
                break;
            }
            case SC_Exit: {
            	int status = machine->ReadRegister(4);
            	if (status == 0){
            		printf("hi from process exit\n");
            	}
            	else{
            		fprintf(stderr, "Process cannot be exited\n");
            	}
            	
            	printf("Current thread name = %s\n", currentThread->getName());
            	if(proc_counter > 0){
            		proc_counter--;
            		printf("SEFFTR\n");
                    //delete currentThread->space;
            		currentThread->Finish();
            	}
            	else{
            		interrupt->Halt();
            	}
                break;
            }
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

