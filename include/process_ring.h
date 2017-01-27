/* process_ring.h
 *
 * Includes declaration for process_rounds and process_rounds_without_semaphore
*/

#ifndef PROCESS_RING_H
#define PROCESS_RING_H
void process_rounds(sid32, sid32, int, int, int, char*, int*);
void process_rounds_without_semaphore(int, int, int*);


#endif
