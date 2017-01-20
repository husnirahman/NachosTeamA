#ifdef CHANGED
#ifndef USERTHREAD_H
#define USERTHREAD_H

#include "copyright.h"
#include "utility.h"


extern int do_UserThreadCreate(int f, int arg);
extern void do_UserThreadExit();
extern void do_UserThreadJoin(int id);
extern int do_userprocess_create(char *filename);
extern void do_UserProcessExit();

#endif	// USERTHREAD_H
#endif	// CHANGED
