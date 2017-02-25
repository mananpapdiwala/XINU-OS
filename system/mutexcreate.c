/*mutexcreate.c - mutex_create*/

#include <xinu.h>

/*------------------------------------------------------------------------
 *  mutexcreate  -  Create a new mutex and returns the status
 *------------------------------------------------------------------------
 */

 syscall mutex_create(
 		mutex_t* lock			/*The pointer to the lock variable*/
 	)
 {
 	*lock = 0;
 	return OK;
 }