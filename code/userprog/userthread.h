#ifdef CHANGED
#ifndef USERTHREAD_H
#define USERTHREAD_H

#include "copyright.h"
#include "utility.h"


extern int do_UserThreadCreate(int f, int arg);
extern void do_UserThreadExit();
extern void do_UserThreadJoin(int id);

#endif	// USERTHREAD_H
#endif	// CHANGED
