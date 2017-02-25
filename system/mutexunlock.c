/*mutexunlock.c - mutex_unlock*/

#include <xinu.h>

/*------------------------------------------------------------------------
 *  mutexunlock  -  unlocks the mutex
 *------------------------------------------------------------------------
 */

 syscall mutex_unlock(
 		mutex_t* lock
 	)
 {
 	*lock = 0;
 	return OK;
 }