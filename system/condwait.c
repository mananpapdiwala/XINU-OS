/*condwait.c - cond_wait*/

#include <xinu.h>

/*------------------------------------------------------------------------
 *  cond_wait  -  Wait on the condition.
 *------------------------------------------------------------------------
 */

 syscall cond_wait(
 		cond_t* cv,
 		mutex_t* lock
 	)
 {
 	mutex_unlock(lock);
 	while((*cv) == 0){
 		printf("");
 	};
 	mutex_lock(lock);
 	*cv = 0;
 	return OK;
 }