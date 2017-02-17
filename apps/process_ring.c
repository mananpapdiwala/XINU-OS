#include <xinu.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <process_ring.h>

void process_rounds(sid32 curSemaphore, sid32 dependentSemaphore, int currentIndex, int processes, int totalRounds, char* programVersion, int *sharedInbox){
        int round_count;
        for(round_count = 0; round_count < totalRounds; round_count++) {
                printf("Break1: %s\n", programVersion);
                wait(dependentSemaphore);
                printf("Break2: %s\n", programVersion);
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
                if(strncmp(programVersion, "loop", 6) == 0){
                        round_count--;
                }
                signal(curSemaphore);   
                printf("Break3: %s\n", programVersion);
                printf("Break4: %p\n", programVersion);
                     
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