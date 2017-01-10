#ifdef CHANGED
#include "copyright.h"
#include "system.h"
#include "synchconsole.h"
#include "synch.h"

static Semaphore *readAvail;
static Semaphore *writeDone;
static Semaphore *locksem;
static void ReadAvail(int arg) { readAvail->V(); }
static void WriteDone(int arg) { writeDone->V(); }

SynchConsole::SynchConsole(char *readFile, char *writeFile)
{
    readAvail = new Semaphore("read avail", 0);
    writeDone = new Semaphore("write done", 0);
    locksem = new Semaphore("locksem", 1);
    console = new Console (readFile, writeFile, ReadAvail, WriteDone, 0);
}
SynchConsole::~SynchConsole()
{
    delete console;
    delete writeDone;
    delete readAvail;
    delete locksem;
}
void SynchConsole::SynchPutChar(const char ch)
{   
    locksem->P();
    console->PutChar(ch);
    writeDone->P ();
    locksem->V();
}
char SynchConsole::SynchGetChar()
{
    readAvail->P ();
    return(console->GetChar());
}
void SynchConsole::SynchPutString(const char s[])
{   
    int i = 0;
    while(s[i]!=EOF && i < MAX_STRING_SIZE){
        SynchPutChar(s[i]);
        i++;
    }
}
void SynchConsole::SynchGetString(char *s, int n)
{
    for(int i =0; i<n; i++){
        s[i]=SynchGetChar();
        //fprintf(stderr, "1%c",s[i]);
        if(s[i]== EOF)
            break;
        
    }
}
void SynchConsole::SynchPutInt(int n){
    char buffer[MAX_STRING_SIZE];
    snprintf(buffer, MAX_STRING_SIZE + 1, "%d", n);
    SynchPutString(buffer);
}

void SynchConsole::SynchGetInt(int *n){
    char buffer[MAX_STRING_SIZE];
    SynchGetString(buffer, MAX_STRING_SIZE);
    sscanf(buffer, "%d", n);
}
#endif // CHANGED