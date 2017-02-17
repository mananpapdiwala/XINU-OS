#include <xinu.h>
#include <stdlib.h>
#include <randdelay.h>

void rand_delay(int ms_max){
	int delay = (rand()+1)%ms_max;

	sleepms(delay);
}