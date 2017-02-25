/*condinit.c - cond_init*/

#include <xinu.h>

/*------------------------------------------------------------------------
 *  cond_init  -  Create a new condition variable and return status
 *------------------------------------------------------------------------
 */

 syscall cond_init(
 		cond_t* cv			/*The pointer to the condition variable*/
 	)
 {
 	*cv = 0;
 	return OK;
 }