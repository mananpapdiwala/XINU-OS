/*xsh_readers_writers.c - readers_writers*/

#include <xinu.h>
#include <stdlib.h>
#include <randdelay.h>
/*------------------------------------------------------------------------
 * xsh_readers_writers - Implements the readers and writers synchronization model.
 *------------------------------------------------------------------------
 */
int counter = 0;
void reader(sid32, sid32, sid32, int, int, int);
void writer(sid32, sid32, sid32, int, int, int);

shellcmd xsh_readers_writers(int nargs, char *args[]){
 	if (nargs == 2 && (strncmp(args[1], "--help", 7) == 0 || strncmp(args[1], "-h", 7) == 0)) {
        printf("Usage: %s\n\n", args[0]);
        printf("Format: ");
        printf("readers_writers <num_writers> <num_readers> <write cycles> <read cycles> <max delay>\n");
        printf("Description:\n");
        printf("\tThe process creates writer and reader processes. The number of writer and reader processes are specified by the users through arguments.\n");
        printf("\tAccepts 5 integer arguments.\n");
        printf("\tThe first argument is the number of writer processes to be created.\n");
        printf("\tThe second argument is the number of reader processes to be created.\n");
        printf("\tThe third argument is the number of write cycles to executed by each writer process.\n");
        printf("\tThe fourth argument is the number of read cycles to executed by each reader process.\n");
        printf("\tThe fifth argument specifies the maximum delay caused by both reader and writer processes.\n");

        return 0;
    }

    if(nargs < 6){
    	fprintf(stderr, "%s: few arguments\n", args[0]);
        fprintf(stderr, "Try '%s --help' for more information\n", args[0]);
        return 1;
    }
    if(nargs > 6){
    	fprintf(stderr, "%s: too many arguments\n", args[0]);
        fprintf(stderr, "Try '%s --help' for more information\n",
        args[0]);
        return 1;
    }

    int writers = atoi(args[1]);
    int readers = atoi(args[2]);
    int writeCycles = atoi(args[3]);
    int readCycles = atoi(args[4]);
    int maxDelay = atoi(args[5]);

    sid32 mutex = semcreate(1);
    sid32 roomEmpty = semcreate(1);
    sid32 turnstile = semcreate(1);

    char processName[10];
    int i = 1;
    for(; i <= writers; i++){
    	sprintf(processName, "writer%d", i);
    	resume(create(writer, 1024, 20, processName, 6, mutex, roomEmpty, turnstile, i, writeCycles, maxDelay));
    }

    i = 1;
    for(; i <= readers; i++){
    	sprintf(processName, "reader%d", i);
    	resume(create(reader, 1024, 20, processName, 6, mutex, roomEmpty, turnstile, i, readCycles, maxDelay));
    }

    return 0;
}

void reader(sid32 mutex, sid32 roomEmpty, sid32 turnstile, int readerNumber, int cycles, int max_delay){
	int i = 1;
	for(; i <= cycles; i++){
		wait(turnstile);
		signal(turnstile);
		wait(mutex);
		counter++;
		if(counter == 1){
			wait(roomEmpty);
		}
		signal(mutex);
		rand_delay(max_delay);
		printf("Reader %d %d\n", readerNumber, i);

		wait(mutex);
		counter--;
		if(counter == 0){
			signal(roomEmpty);
		}	
		signal(mutex);
	}
	

}

void writer(sid32 mutex, sid32 roomEmpty, sid32 turnstile, int writerNumber, int cycles, int max_delay){
	int i = 1;
	for(; i <= cycles; i++){
		wait(turnstile);
		wait(roomEmpty);
		rand_delay(max_delay);
		printf("Writer %d %d\n", writerNumber, i);
		signal(turnstile);
		signal(roomEmpty);
	}

}