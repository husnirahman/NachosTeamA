#ifdef CHANGED
#include "copyright.h"
#include "system.h"
#include "synchconsole.h"
#include "synch.h"

static Semaphore *readAvail;
static Semaphore *writeDone;
static Lock *lockRead;
static Lock *lockWrite;
static Lock *lockReadS;
static Lock *lockWriteS;
static void ReadAvail(int arg) { readAvail->V(); }
static void WriteDone(int arg) { writeDone->V(); }

//----------------------------------------------------------------------
// SynchConsole::SynchConsole
//      Initialize a synchconsole with a console for the input and output
//      and read/write locks for the operations.
//----------------------------------------------------------------------
SynchConsole::SynchConsole(char *readFile, char *writeFile)
{
    readAvail = new Semaphore("read avail", 0);
    writeDone = new Semaphore("write done", 0);
    lockRead = new Lock("lock read");
    lockWrite = new Lock("lock write");
    lockReadS = new Lock("lock read string");
    lockWriteS = new Lock("locks write string");
    console = new Console (readFile, writeFile, ReadAvail, WriteDone, 0);
}

//----------------------------------------------------------------------
// SynchConsole::~SynchConsole
//      De-allocate the memory used by synchconsole
//----------------------------------------------------------------------
SynchConsole::~SynchConsole()
{
    delete console;
    delete writeDone;
    delete readAvail;
    delete lockRead;
    delete lockWrite;
    delete lockReadS;
    delete lockWriteS;
}

//----------------------------------------------------------------------
// SynchConsole::SynchPutChar
//      Writes the character "ch" to the console output
//----------------------------------------------------------------------
void SynchConsole::SynchPutChar(const char ch)
{   
    lockWrite->Acquire();
    console->PutChar(ch);
    writeDone->P ();
    lockWrite->Release();
}

//----------------------------------------------------------------------
// SynchConsole::InterGetChar
//      Reads a character from the console output and return its value.
//      Used by both SynchGetChar and SynchGetString to call the console operation
//----------------------------------------------------------------------
char SynchConsole::InterGetChar()
{
    lockRead->Acquire();
    readAvail->P ();
    char c = console->GetChar();
    lockRead->Release();
    return c;
}

//----------------------------------------------------------------------
// SynchConsole::SynchGetChar
//      Calls the operation InterGetChar and encapsulates with a condition.
//      Ignores the read of "ENTER" / "\n"
//----------------------------------------------------------------------
char SynchConsole::SynchGetChar()
{
    char c = InterGetChar();
    while(c=='\n')
     	c = InterGetChar();
    return c;
}


//----------------------------------------------------------------------
// SynchConsole::SynchPutString
//      Writes a string to the console output
//----------------------------------------------------------------------
void SynchConsole::SynchPutString(const char s[])
{   
    int i = 0;
    while(s[i]!='\0' && i < MAX_STRING_SIZE){
        SynchPutChar(s[i]);
        i++;
    }
}

//----------------------------------------------------------------------
// SynchConsole::SynchGetString
//      Reads a string with "n" number of character and stores it in "s"
//----------------------------------------------------------------------
void SynchConsole::SynchGetString(char *s, int n)
{
    for(int i =0; i<n; i++){
        s[i]=InterGetChar();
         if(s[i]== '\0')
            break;
        
    }
}

//----------------------------------------------------------------------
// SynchConsole::SynchPutInt
//      Writes an integer to the console output
//----------------------------------------------------------------------
void SynchConsole::SynchPutInt(int n){
	char *buffer = new char[MAX_STRING_SIZE];
    snprintf(buffer, MAX_STRING_SIZE + 1, "%d", n);
    SynchPutString(buffer);
    delete buffer;
}

//----------------------------------------------------------------------
// SynchConsole::SynchGetInt
//      Reads an integer from the console input
//----------------------------------------------------------------------
void SynchConsole::SynchGetInt(int *n){
    char *buffer = new char[MAX_STRING_SIZE];
    SynchGetString(buffer, MAX_STRING_SIZE);
    //printf("Check = %s\n",buffer);
    sscanf(buffer, "%d", n);
    delete buffer;
}
#endif // CHANGED
