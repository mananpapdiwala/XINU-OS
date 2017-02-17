/*xsh_producer_consumer.c - producer_consumer*/

#include <xinu.h>
#include <stdint.h>
#include <stdlib.h>
#include <limits.h>
#include <randdelay.h>
/*------------------------------------------------------------------------
 * xsh_producer_consumer - Creates a finite buffer and implements the model of prodcuer and consumer.
 *------------------------------------------------------------------------
 */
uint8_t buffer[1000];
int front = 0;
int rear = 0;
int itemCount = 0;
int bufferSize;

void producerEnqueue(uint8_t);
uint8_t consumerDequeue();
void process_producer(sid32, sid32, sid32, int, int);
void process_consumer(sid32, sid32, sid32, int, int);

shellcmd xsh_producer_consumer(int nargs, char *args[]){
	if (nargs == 2 && (strncmp(args[1], "--help", 7) == 0 || strncmp(args[1], "-h", 7) == 0)) {
        printf("Usage: %s\n\n", args[0]);
        printf("Format: ");
        printf("producer_consumer <buffer size in bytes> <bytes to send> <max delay>\n");
        printf("Description:\n");
        printf("\tThe process creates a buffer of a finite size and implements the producer consumer model to add and remove data from the buffer.\n");
        printf("\tAccepts 3 integer arguments.\n");
        printf("\tThe first argument is the size of the buffer to be created.\n");
        printf("\tThe second argument specifies the bytes to be created by producer and consumed by consumer.\n");
        printf("\tThe third argument specifies the maximum delay caused by both consumer and producer.\n");

        return 0;
        }

    if(nargs < 4){
    	fprintf(stderr, "%s: few arguments\n", args[0]);
        fprintf(stderr, "Try '%s --help' for more information\n", args[0]);
        return 1;
    }
    if(nargs > 4){
    	fprintf(stderr, "%s: too many arguments\n", args[0]);
        fprintf(stderr, "Try '%s --help' for more information\n",
        args[0]);
        return 1;
    }

    bufferSize = atoi(args[1]);
    if(bufferSize > 1000 || bufferSize < 0){
    	fprintf(stderr, "%s: Buffer size should between 1 and 1000.\n", args[0]);
    	return 1;
    }
    int numberOfBytes = atoi(args[2]);
    int maxDelay = atoi(args[3]);

    sid32 itemsemaphore = semcreate(0);
    sid32 spacesemaphore = semcreate(bufferSize);
    sid32 mutexsemaphore = semcreate(1);
    resume(create(process_producer, 1024, 20, "Producer", 5, itemsemaphore, spacesemaphore, mutexsemaphore, numberOfBytes, maxDelay));
    resume(create(process_consumer, 1024, 20, "Consumer", 5, itemsemaphore, spacesemaphore, mutexsemaphore, numberOfBytes, maxDelay));


    return 0;

}

void process_producer(sid32 items, sid32 spaces, sid32 mutex, int nb, int max_delay){
	uint8_t byteData = 0;
	int i = 0;
	for(; i < nb; i++){
		wait(spaces);
		wait(mutex);
		rand_delay(max_delay);
		printf("Producer: %d\n", byteData);
		producerEnqueue(byteData++);
		signal(mutex);
		signal(items);
	}
}

void process_consumer(sid32 items, sid32 spaces, sid32 mutex, int nb, int max_delay){
	int i = 0;
	for(; i < nb; i++){
		wait(items);
		wait(mutex);
		rand_delay(max_delay);
		printf("Consumer: %d\n", consumerDequeue());
		signal(mutex);
		signal(spaces);

	}
}

/*int size(){
	return itemCount;
} 

int8_t isEmpty(){
	return(size() == 0);
}

int8_t isFull(){
	return(size() == bufferSize);
}*/

void producerEnqueue(uint8_t data){
	itemCount++;
	buffer[rear] = data;
	rear = (rear+1)%bufferSize;
}

uint8_t consumerDequeue(){
	itemCount--;
	uint8_t data = buffer[front];
	buffer[front] = -1;
	front = (front+1)%bufferSize;
	return data;
}