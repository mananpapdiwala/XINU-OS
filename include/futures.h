#ifndef _FUTURE_H_
/* futures.h - future_alloc, future_free, future_get, future_set */

/* Contains data tpyes and functions for futures */

#define _FUTURE_H_  

typedef enum {
  FUTURE_EMPTY,
  FUTURE_WAITING,
  FUTURE_READY
} future_state_t;

typedef enum {
  FUTURE_EXCLUSIVE,
  FUTURE_SHARED,
  FUTURE_QUEUE
} future_mode_t;

typedef struct{
  pid32 key;
  struct future_queue_node *next;
} future_queue_node;

typedef struct {
  int value;
  future_state_t state;
  future_mode_t mode;
  pid32 pid;
  future_queue_node* set_queue;
  future_queue_node* get_queue;
  int get_queue_count;
} future_t;

/* Interface for the Futures system calls */
future_t* future_alloc(future_mode_t mode);
syscall future_free(future_t*);
syscall future_get(future_t*, int*);
syscall future_set(future_t*, int);

uint future_prod(future_t*, int);
uint future_cons(future_t*);

#endif /* _FUTURE_H_ */