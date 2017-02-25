/*condsignal.c - cond_signal*/

#include <xinu.h>

/*------------------------------------------------------------------------
 *  cond_signal  -  Wake up at least one of the processes waiting on the condition variable.
 *------------------------------------------------------------------------
 */

 syscall cond_signal(
 		cond_t* cv
 	)
 {
 	*cv = 1;
 	return OK;
 }