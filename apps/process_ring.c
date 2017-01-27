#include <xinu.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <process_ring.h>

void process_rounds(sid32 curSemaphore, sid32 dependentSemaphore, int currentIndex, int processes, int totalRounds, char *programVersion, int *sharedInbox){
        int round_count;
        for(round_count = 0; round_count < totalRounds; round_count++) {
                wait(dependentSemaphore);
                printf("Ring Element %d : Round %d : Value : %d\n", currentIndex, round_count, sharedInbox[currentIndex]);  
                if(currentIndex == (processes-1)){
                        sharedInbox[0] = sharedInbox[currentIndex] - 1;
                }
                else{
                        sharedInbox[currentIndex + 1] = sharedInbox[currentIndex]-1;
                }

                if(sharedInbox[currentIndex] - 1 == 0){
                        printf("ZERO!\n");
                }
                signal(curSemaphore);   
                if(strncmp(programVersion, "loop", 15) == 0){
                        round_count--;
                }     
        }
        
}

void process_rounds_without_semaphore(int currentIndex, int totalRounds, int *sharedInbox){
        int round_count;
        for(round_count = 0; round_count < totalRounds; round_count++) {
                printf("Ring Element %d : Round %d : Value : %d\n", currentIndex, round_count, *sharedInbox);  
                (*sharedInbox)--;

                if(*sharedInbox == 0){
                        printf("ZERO!\n");
                }
        }
}