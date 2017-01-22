/* Start.s 
 *	Assembly language assist for user programs running on top of Nachos.
 *
 *	Since we don't want to pull in the entire C library, we define
 *	what we need for a user program here, namely Start and the system
 *	calls.
 */

#include "syscall.h"

        .text   
        .align  2

/* -------------------------------------------------------------
 * __start
 *	Initialize running a C program, by calling "main". 
 *
 * 	NOTE: This has to be first, so that it gets loaded at location 0.
 *	The Nachos kernel always starts a program by jumping to location 0.
 * -------------------------------------------------------------
 */

	.globl __start
	.ent	__start
__start:
	jal	main
	move	$4,$0		
	jal	Exit	 /* if we return from main, exit(0) */
	.end __start

/* -------------------------------------------------------------
 * System call stubs:
 *	Assembly language assist to make system calls to the Nachos kernel.
 *	There is one stub per system call, that places the code for the
 *	system call into register r2, and leaves the arguments to the
 *	system call alone (in other words, arg1 is in r4, arg2 is 
 *	in r5, arg3 is in r6, arg4 is in r7)
 *
 * 	The return value is in r2. This follows the standard C calling
 * 	convention on the MIPS.
 * -------------------------------------------------------------
 */

	.globl Halt
	.ent	Halt
Halt:
	addiu $2,$0,SC_Halt
	syscall
	j	$31
	.end Halt

#ifdef CHANGED
        .globl PutChar
	.ent	PutChar
PutChar:
	addiu $2,$0,SC_PutChar
	syscall
	j	$31
	.end PutChar
	
	.globl GetChar
	.ent	GetChar
GetChar:
	addiu $2,$0,SC_GetChar
	syscall
	j	$31
	.end GetChar
	
	.globl Puts
	.ent	Puts
Puts:
	addiu $2,$0,SC_Puts
	syscall
	j	$31
	.end Puts
	
	.globl Gets
	.ent	Gets
Gets:
	addiu $2,$0,SC_Gets
	syscall
	j	$31
	.end Gets
	
	.globl UserThreadCreate
	.ent	UserThreadCreate
	
	.globl PutInt
	.ent	PutInt
PutInt:
	addiu $2,$0,SC_PutInt
	syscall
	j	$31
	.end PutInt

	.globl GetInt
	.ent	GetInt
GetInt:
	addiu $2,$0,SC_GetInt
	syscall
	j	$31
	.end GetInt
	
UserThreadCreate:
	addiu $2,$0,SC_ThdCreate
	syscall
	j	$31
	.end UserThreadCreate
	
	.globl UserThreadExit
	.ent	UserThreadExit
UserThreadExit:
	addiu $2,$0,SC_ThdExit
	syscall
	j	$31
	.end UserThreadExit
	
	.globl UserThreadJoin
	.ent	UserThreadJoin
UserThreadJoin:
	addiu $2,$0,SC_ThdJoin
	syscall
	j	$31
	.end UserThreadJoin
	
	.globl ForkExec
	.ent	ForkExec
ForkExec:
	addiu $2,$0,SC_ForkE
	syscall
	j	$31
	.end ForkExec
	
	.globl createDir
	.ent	createDir
createDir:
	addiu $2,$0,SC_createDir
	syscall
	j	$31
	.end createDir
	
	.globl changeDir
	.ent	changeDir
changeDir:
	addiu $2,$0,SC_changeDir
	syscall
	j	$31
	.end changeDir
	
	.globl displayDir
	.ent	displayDir
displayDir:
	addiu $2,$0,SC_displayDir
	syscall
	j	$31
	.end displayDir
	
	.globl fopen
	.ent	fopen
fopen:
	addiu $2,$0,SC_fopen
	syscall
	j	$31
	.end fopen
	
	.globl fread
	.ent	fread
fread:
	addiu $2,$0,SC_fread
	syscall
	j	$31
	.end fread
	
	.globl fwrite
	.ent	fwrite
fwrite:
	addiu $2,$0,SC_fwrite
	syscall
	j	$31
	.end fwrite
	
	.globl fclose
	.ent	fclose
fclose:
	addiu $2,$0,SC_fclose
	syscall
	j	$31
	.end fclose
	
#endif 
	.globl Exit
	.ent	Exit
Exit:
	addiu $2,$0,SC_Exit
	syscall
	j	$31
	.end Exit

	.globl Exec
	.ent	Exec
Exec:
	addiu $2,$0,SC_Exec
	syscall
	j	$31
	.end Exec

	.globl Join
	.ent	Join
Join:
	addiu $2,$0,SC_Join
	syscall
	j	$31
	.end Join

	.globl Create
	.ent	Create
Create:
	addiu $2,$0,SC_Create
	syscall
	j	$31
	.end Create

	.globl Open
	.ent	Open
Open:
	addiu $2,$0,SC_Open
	syscall
	j	$31
	.end Open

	.globl Read
	.ent	Read
Read:
	addiu $2,$0,SC_Read
	syscall
	j	$31
	.end Read

	.globl Write
	.ent	Write
Write:
	addiu $2,$0,SC_Write
	syscall
	j	$31
	.end Write

	.globl Close
	.ent	Close
Close:
	addiu $2,$0,SC_Close
	syscall
	j	$31
	.end Close

	.globl Fork
	.ent	Fork
Fork:
	addiu $2,$0,SC_Fork
	syscall
	j	$31
	.end Fork

	.globl Yield
	.ent	Yield
Yield:
	addiu $2,$0,SC_Yield
	syscall
	j	$31
	.end Yield

/* dummy function to keep gcc happy */
        .globl  __main
        .ent    __main
__main:
        j       $31
        .end    __main

