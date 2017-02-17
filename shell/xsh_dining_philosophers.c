/*xsh_dining_philosophers.c - dining_philosophers*/

#include <xinu.h>
#include <stdlib.h>
#include <randdelay.h>
/*------------------------------------------------------------------------
 * xsh_dining_philosophers - Implements the dining philosophers synchronization model.
 *------------------------------------------------------------------------
 */
int philosophers;
int left(int);
int right(int);
void think(int, int, int);
void eat(int, int, int);
void get_forks(int, sid32, sid32 forkArr[]);
void put_forks(int, sid32, sid32 forkArr[]);
void philosopher_tasks(int, int, int, sid32, sid32 forks[]);

shellcmd xsh_dining_philosophers(int nargs, char *args[]){
 	if (nargs == 2 && (strncmp(args[1], "--help", 7) == 0 || strncmp(args[1], "-h", 7) == 0)) {
        printf("Usage: %s\n\n", args[0]);
        printf("Format: ");
        printf("dining_philosophers <num philosophers> <num_cycles> <max delay>\n");
        printf("Description:\n");
        printf("\tThe process creates philosopher processes. The number of philosopher processes are specified by the users through arguments.\n");
        printf("\tAccepts 3 integer arguments.\n");
        printf("\tThe first argument is the number of philosopher processes to be created.\n");
        printf("\tThe second argument is the number of cycles to executed by each philosopher process.\n");
        printf("\tThe third argument specifies the maximum delay caused by philosopher processes.\n");

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

    philosophers = atoi(args[1]);
    int philosopherCycles = atoi(args[2]);
    int maxDelay = atoi(args[3]);

    sid32 forks[philosophers];
    int i = 0;
    for(; i < philosophers; i++){
    	forks[i] = semcreate(1);
    }

    sid32 footman = semcreate(philosophers-1);
    i = 1;
    char philosopherName[20];
    for(; i <= philosophers; i++){
    	sprintf(philosopherName, "philosopher%d", i);
    	resume(create(philosopher_tasks, 1024, 20, philosopherName, 5, i, philosopherCycles, maxDelay, footman, forks));
    }

    return 0;

}

int left(int position){
	return position;
}

int right(int position){
	return (position+1)%philosophers;
}

void get_forks(int position, sid32 footman, sid32 forkArr[]){
	wait(footman);
	wait(forkArr[right(position)]);
	wait(forkArr[left(position)]);
}

void put_forks(int position, sid32 footman, sid32 forkArr[]){
	signal(forkArr[right(position)]);
	signal(forkArr[left(position)]);
	signal(footman);
}

void philosopher_tasks(int philosopherPosition, int cycles, int max_delay, sid32 footman, sid32 forks[]){
	int i = 1;
	for(; i <= cycles; i++){
		think(philosopherPosition, i, max_delay);
		get_forks(philosopherPosition, footman, forks);
		eat(philosopherPosition, i, max_delay);
		put_forks(philosopherPosition, footman, forks);
	}
}

void think(int philosopherNumber, int cycleNumber, int max_delay){
	rand_delay(max_delay);
	printf("Philosopher %d thinking %d\n", philosopherNumber, cycleNumber);
}

void eat(int philosopherNumber, int cycleNumber, int max_delay){
	rand_delay(max_delay);
	printf("Philosopher %d eating %d\n", philosopherNumber, cycleNumber);
}