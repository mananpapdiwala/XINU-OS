 /*xsh_process_ring.c - process_ring */

#include <xinu.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <process_ring.h>

/*------------------------------------------------------------------------
 * xsh_process_ring - Count downs a number equal to number of processes times the number of rounds.
 *------------------------------------------------------------------------
 */

shellcmd xsh_process_ring(int nargs, char *args[]) {

        /* Output info for '--help' argument */
        int num_process = 4;
        int rounds = 5;
        char version[6];
        int inbox[NPROC];
        strncpy(version, "work", 10);
        if (nargs == 2 && (strncmp(args[1], "--help", 7) == 0 || strncmp(args[1], "-h", 7) == 0)) {
                printf("Usage: %s\n\n", args[0]);
                printf("Description:\n");
                printf("\tThe processes coordinate with each other to count down a number from its starting value and prints the value after each decrementation.\n");
                printf("\tThe initial value of the number is set to a value which is equal to multiplication of number of processes and number of rounds.\n");
                printf("\tIn each round each processes run in a order and decrement the value of the number by 1 and displays the number.\n");
                printf("\tWhen all the rounds are finished the final value of the number is zero.\n");

                printf("Options (can be used in combination):\n");
                printf("\t-p or --processes int\tSets the number of processes. Default value is 4. Maximum is 100.\n");
                printf("\t-r or --round int\tSets the number of rounds. Default value is 5.\n");
                printf("\t-v or --version mode\tHere mode can be worok, hang, loop and chaos. Default mode is work.\n");
                printf("\t--help\tdisplay this help and exit\n");
                return 0;
        }
        if (nargs > 1){
                int i;
                for(i=1; i < nargs; i++){
                        if (strncmp(args[i], "--processes", 15) == 0 || strncmp(args[i], "-p", 15) == 0){
                                num_process = atoi(args[++i]); 
                                if(num_process <= 0 || num_process > NPROC){
                                        fprintf(stderr, "%s: Please enter a valid value for number of processes.\n", args[0]);
                                        fprintf(stderr, "Try %s: --help or -h for more information\n", args[0]);
                                        return 1;
                                }                          
                        }
                        else if (strncmp(args[i], "--round", 15) == 0 || strncmp(args[i], "-r", 15) == 0){
                                rounds = atoi(args[++i]);
                                if(rounds <= 0){
                                        fprintf(stderr, "%s: The value of rounds exceeds maximum integer value.\n", args[0]);
                                        fprintf(stderr, "Try %s: --help or -h for more information\n", args[0]);
                                        return 1;
                                }
                        }
                        else if (strncmp(args[i], "--version", 15) == 0 || strncmp(args[i], "-v", 15) == 0){
                                if(strncmp(args[i+1], "work", 15) == 0 || strncmp(args[i+1], "hang", 15) == 0 || strncmp(args[i+1], "loop", 15) == 0 || strncmp(args[i+1], "chaos", 15) == 0){
                                        strncpy(version, args[++i], 10);
                                }
                                else{
                                        fprintf(stderr, "%s: Invalid mode. Please enter a valid mode.\n", args[0]);
                                        fprintf(stderr, "Try %s: --help or -h for more information\n", args[0]);
                                        return 1;
                                }

                        }
                        else{
                                fprintf(stderr, "%s: Invalid Arguments.\n", args[0]);
                                fprintf(stderr, "Try %s: --help or -h for more information\n", args[0]);
                                return 1;
                        }
                }
                int sum = 0;
                for (i = 0; i < num_process; i++){
                        sum+=rounds;
                        if(sum <= 0){
                                fprintf(stderr, "%s: Invalid Arguments.\n", args[0]);
                                fprintf(stderr, "Try %s: --help or -h for more information\n", args[0]);
                                return 1;        
                        }
                }
        }
        sid32 semaphores[num_process];
        int i;
        char processName[num_process];
        if (strncmp(version, "work", 15) == 0 || strncmp(version, "hang", 15) == 0 || strncmp(version, "loop", 15) == 0){
                for(i = 0; i < num_process; i++){
                        inbox[i] = num_process * rounds;
                        if (i == (num_process-1) && (strncmp(version, "work", 15) == 0 || strncmp(version, "loop", 15) == 0)){
                                semaphores[i] = semcreate(1);
                        }
                        else{
                                semaphores[i] = semcreate(0);
                        }
                }
                
                for(i = 0; i < num_process; i++){
                        sprintf(processName, "process%d",i);
                        if(i == 0){
                                resume(create(process_rounds, 1024, 20, processName, 7, semaphores[i], semaphores[num_process-1], i, num_process, rounds, version, inbox));        
                        }
                        else{
                                resume(create(process_rounds, 1024, 20, processName, 7, semaphores[i], semaphores[i-1], i, num_process, rounds, version, inbox));        
                        }       
                }
        }
        if (strncmp(version, "chaos", 15) == 0){
                for(i = 0; i < num_process; i++){
                        inbox[i] = num_process * rounds;
                        sprintf(processName, "process%d", i);
                        resume(create(process_rounds_without_semaphore, 1024, 20, processName, 3, i, rounds, inbox));
                }
        }


        if(strncmp(version, "hang", 15) == 0){
                wait(semaphores[0]);
        }
        
        return 0;
}