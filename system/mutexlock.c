/*mutexlock.c - mutex_lock*/

#include <xinu.h>

/*------------------------------------------------------------------------
 *  mutexlock  -  Locks the mutex and if the process is unable to set the lock, it makes the process busy wait.
 *------------------------------------------------------------------------
 */

 syscall mutex_lock(
 		mutex_t* lock
 	)
 {
 	while(testandset(lock));
 	return OK;
 }