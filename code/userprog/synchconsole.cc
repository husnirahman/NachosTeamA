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
void SynchConsole::SynchPutChar(const char ch)
{   
    lockWrite->Acquire();
    console->PutChar(ch);
    writeDone->P ();
    lockWrite->Release();
}
char SynchConsole::SynchGetChar()
{
    lockRead->Acquire();
    readAvail->P ();
    char c = console->GetChar();
    lockRead->Release();
    return c;
}
void SynchConsole::SynchPutString(const char s[])
{   
    int i = 0;
    while(s[i]!='\0' && i < MAX_STRING_SIZE){
        SynchPutChar(s[i]);
        i++;
    }
}
void SynchConsole::SynchGetString(char *s, int n)
{
    for(int i =0; i<n; i++){
        s[i]=SynchGetChar();
         if(s[i]== '\0')
            break;
        
    }
}
void SynchConsole::SynchPutInt(int n){
	char *buffer = new char[MAX_STRING_SIZE];
    snprintf(buffer, MAX_STRING_SIZE + 1, "%d", n);
    SynchPutString(buffer);
    delete buffer;
}

void SynchConsole::SynchGetInt(int *n){
    char *buffer = new char[MAX_STRING_SIZE];
    SynchGetString(buffer, MAX_STRING_SIZE);
    sscanf(buffer, "%d", n);
    delete buffer;
}
#endif // CHANGED
