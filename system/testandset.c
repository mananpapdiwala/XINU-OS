/*testandset.c - testandset*/

#include <xinu.h>

/*------------------------------------------------------------------------
 *  testandset  -  Locks the mutex and returns prev lock value.
 *------------------------------------------------------------------------
 */

 mutex_t testandset(
 	mutex_t* lock
 	)
 {
 	intmask mask = disable();
 	mutex_t* prev;
 	*prev = *lock;
 	*lock = 1;
 	restore(mask);
 	return *prev;
 }