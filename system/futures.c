#include <xinu.h>
#include <futures.h>

future_t* future_alloc(future_mode_t mode){
	if(mode != FUTURE_EXCLUSIVE && mode != FUTURE_SHARED && mode != FUTURE_QUEUE)
		return NULL;
	future_t* new_future = (future_t*)getmem(sizeof(future_t));

	new_future->mode = mode;
	new_future->state = FUTURE_EMPTY;
	new_future->set_queue = (future_queue_node*)getmem(sizeof(future_queue_node));
	new_future->set_queue->key = NULL;
	new_future->set_queue->next = NULL;
	new_future->get_queue = (future_queue_node*)getmem(sizeof(future_queue_node));
	new_future->get_queue->key = NULL;
	new_future->get_queue->next = NULL;
	new_future->get_queue_count = 0;
	//new_future->value = NULL;
	return new_future;
}

syscall future_free(future_t* f){
	future_queue_node* curr, * next;
	curr = f->set_queue;
	while(curr->next != NULL){
		next = curr->next;
		freemem(curr, sizeof(future_queue_node));
		curr = next;
	}
	freemem(curr, sizeof(future_queue_node));

	curr = f->get_queue;
	while(curr->next != NULL){
		next = curr->next;
		freemem(curr, sizeof(future_queue_node));
		curr = next;
	}
	freemem(curr, sizeof(future_queue_node));
	freemem(f, sizeof(future_t));
	return OK;
}

syscall future_get(future_t* f, int* value){
	intmask mask;
	struct procent *prptr;

	mask = disable();
	if(!f || !value){
		restore(mask);
		return SYSERR;
	}

	switch(f->mode){
		case FUTURE_EXCLUSIVE:
			if(f->get_queue->key != NULL){
					restore(mask);
					return SYSERR;
			}
			else{
				f->get_queue->key = currpid;
				if(f->set_queue->key == NULL){
					prptr = &proctab[currpid];
					prptr->prstate = PR_WAIT;
					f->state = FUTURE_WAITING;
					resched();	
					*value = f->value;
					f->state = FUTURE_EMPTY;
				}
				else{
					*value = f->value;
					f->state = FUTURE_EMPTY;
					ready(f->set_queue->key);	
				}
				
				f->get_queue->key = NULL;
				f->set_queue->key = NULL;
			}

			break;
		case FUTURE_SHARED:
			if(f->set_queue->key != NULL){
				*value = f->value;
				f->state = FUTURE_EMPTY;
				f->get_queue->key = NULL;
				f->set_queue->key = NULL;
				ready(f->set_queue->key);
			}
			else{
				future_queue_node* new_node = (future_queue_node*)getmem(sizeof(future_queue_node));
				new_node->key = currpid;
				new_node->next = NULL;
				if(f->get_queue_count == 0){
					f->get_queue = new_node;
				}
				else{
					future_queue_node* temp;
					temp = f->get_queue;
					while(temp->next != NULL){
						temp = temp->next;
					}
					temp->next = new_node;
					temp = NULL;
				}
				new_node = NULL;
				f->get_queue_count++;
				prptr = &proctab[currpid];
				prptr->prstate = PR_WAIT;
				resched();
				*value = f->value;
				f->state = FUTURE_EMPTY;
				if(--(f->get_queue_count)==0){
					f->set_queue->key = NULL;
				}
			}
			break;
		case FUTURE_QUEUE:
			if(f->get_queue->key != NULL){
				future_queue_node* new_node = (future_queue_node*)getmem(sizeof(future_queue_node));
				new_node->key = currpid;
				new_node->next = NULL;
				future_queue_node* temp;
				temp = f->get_queue;
				while(temp->next != NULL){
					temp = temp->next;
				}
				temp->next = new_node;
				temp = NULL;
				new_node = NULL;
				prptr = &proctab[currpid];
				prptr->prstate = PR_WAIT;
				resched();

				//future_queue_node* temp;
				pid32 waitProcess = f->set_queue->key;
				if(f->set_queue->next!=NULL){
					temp = f->set_queue->next;
					freemem(f->set_queue, sizeof(future_queue_node));
					f->set_queue = temp;
					temp = NULL;
				}
				else{
					f->set_queue->key = NULL;
					f->set_queue->next = NULL;
				}
				*value = f->value;
				f->state = FUTURE_EMPTY;
			}
			else{
				if(f->set_queue->key == NULL){
					f->get_queue->key = currpid;
					f->get_queue->next = NULL;
					f->state = FUTURE_WAITING;
					prptr = &proctab[currpid];
					prptr->prstate = PR_WAIT;
					resched();

					future_queue_node* temp1;

					pid32 waitProcess = f->set_queue->key;
					if(f->set_queue->next!=NULL){
						temp1 = f->set_queue->next;
						freemem(f->set_queue, sizeof(future_queue_node));
						f->set_queue = temp1;
						temp1 = NULL;
					}
					else{
						f->set_queue->key = NULL;
						f->set_queue->next = NULL;
					}
					*value = f->value;
					f->state = FUTURE_EMPTY;
				}
				else{
					future_queue_node* new_node = (future_queue_node*)getmem(sizeof(future_queue_node));
					new_node->key = currpid;
					new_node->next = NULL;
					future_queue_node* temp2;
					if(f->get_queue->key == NULL){
						f->get_queue->key = currpid;
						f->get_queue->next = NULL;
					}
					else{
						temp2 = f->get_queue;
						while(temp2->next != NULL){
							temp2 = temp2->next;
						}
						temp2->next = new_node;
					}
					temp2 = NULL;
					new_node = NULL;


					pid32 waitProcess = f->set_queue->key;
					if(f->set_queue->next!=NULL){
						temp2 = f->set_queue->next;
						freemem(f->set_queue, sizeof(future_queue_node));
						f->set_queue = temp2;
						temp2 = NULL;
					}
					else{
						f->set_queue->key = NULL;
						f->set_queue->next = NULL;
					}
					prptr = &proctab[currpid];
					prptr->prstate = PR_WAIT;
					ready(waitProcess);
					*value = f->value;
					f->state = FUTURE_EMPTY;

					if(f->get_queue->key != NULL){
						ready(f->get_queue->key);
					}
				}
			}

			break;
		default:
			restore(mask);
			return SYSERR;
	}

	restore(mask);
	return OK;
}

syscall future_set(future_t* f, int value){
	intmask mask;
	struct procent *prptr;
	mask = disable();

	if(!f || !value){
		restore(mask);
		return SYSERR;
	}
	switch(f->mode){
		case FUTURE_EXCLUSIVE:
			if(f->set_queue->key != NULL){
				restore(mask);
				return SYSERR;
			}
			else{
				f->set_queue->key = currpid;
				f->value = value;
				f->state = FUTURE_READY;
				if(f->get_queue->key == NULL){
					prptr = &proctab[currpid];
					prptr->prstate = PR_WAIT;
					resched();
				}
				else{
					ready(f->get_queue->key);
				}
			}

			break;
		case FUTURE_SHARED:
			if(f->set_queue->key != NULL){
				restore(mask);
				return SYSERR;
			}
			else{
				f->set_queue->key = currpid;
				f->value = value;
				f->state = FUTURE_READY;
				if(f->get_queue->key == NULL){
					prptr = &proctab[currpid];
					prptr->prstate = PR_WAIT;
					resched();
				}
				else{
					resched_cntl(DEFER_START);
					future_queue_node* curr, * next;
					curr = f->get_queue;
					while(curr->next != NULL){
						next = curr->next;
						ready(curr->key);
						freemem(curr, sizeof(future_queue_node));
						curr = next;
					}

					ready(curr->key);
					f->get_queue = curr;
					f->get_queue->key = NULL;
					f->get_queue->next = NULL;					
					resched_cntl(DEFER_STOP);
				}
				break;
				
			}
		case FUTURE_QUEUE:
			if(f->set_queue->key != NULL){
				future_queue_node* new_node  = (future_queue_node*)getmem(sizeof(future_queue_node));
				new_node->key = currpid;
				new_node->next = NULL;
				future_queue_node* temp;
				temp = f->set_queue;
				while(temp->next!=NULL){
					temp = temp->next;
				}
				temp->next = new_node;
				temp = NULL;
				new_node = NULL;
				prptr = &proctab[currpid];
				prptr->prstate = PR_WAIT;
				resched();
			}
			else{
				if(f->get_queue->key == NULL){
					f->set_queue->key = currpid;
					f->set_queue->next = NULL;
					f->state = FUTURE_READY;
					prptr = &proctab[currpid];
					prptr->prstate = PR_WAIT;
					resched();
				}
			}
			
			future_queue_node* temp1;
			f->value = value;
			f->state = FUTURE_READY;
			pid32 waitProcess = f->get_queue->key;
			if(f->get_queue->next != NULL){
				temp1 = f->get_queue->next;
				freemem(f->get_queue, sizeof(future_queue_node));
				f->get_queue = temp1;
				temp1 = NULL;
			}
			else{
				f->get_queue->key == NULL;
				f->get_queue->next = NULL;
			}
			//printf("Producer waking %d\n", waitProcess);
			ready(waitProcess);

			break;
		default:
			restore(mask);
			return SYSERR;
	}

	restore(mask);
	return OK;
}