/*xsh_babybird.c - babybird*/
#include <xinu.h>
#include <stdlib.h>

/*------------------------------------------------------------------------
 * xsh_babybird - Creates baby bird that tskes turn eating worms from common dish.
 *------------------------------------------------------------------------
 */
int worms_on_dish;
int total_worms;
int worm_count;
int num_baby_birds;
int num_eat_worms;
int num_fetch_worms;
    
cond_t fm;
cond_t fd;
pid32 prev;

void fetch_worms(mutex_t*, cond_t*, cond_t*);
void eat_worms(int, mutex_t*, cond_t*, cond_t*);

shellcmd xsh_babybird(int nargs, char *args[]) {

 	if (nargs == 2 && (strncmp(args[1], "--help", 7) == 0 || strncmp(args[1], "-h", 7) == 0)) {
                printf("Usage: %s\n\n", args[0]);
                printf("Format: ");
        		printf("babybird -b or --babies <num_baby_birds> -e or --eat <num_eat_worms> -f or --fetch <num_fetch_worms>\n");
        		printf("The order of parameters babies, eat and fetch do not matter.\n");
                printf("Description:\n");
                printf("\tCreates baby birds that takes turn eating worms from common dish.\n");
                printf("\tThe paramater --babies or -b corresponds to the number of baby birds. The program creates these many number of baby bird processes.\n");
                printf("\tThe parameter --eat or -e corresponds to number of worms each baby bird is suppose to eat till end.\n");
                printf("\tThe parameter --fetch or -f corresponds to number of worms parent bird can fetch in one turn.\n");

                return 0;
    }

    if(nargs < 7){
    	fprintf(stderr, "%s: few arguments\n", args[0]);
        fprintf(stderr, "Try '%s --help' for more information\n", args[0]);
        return 1;
    }
    if(nargs > 7){
    	fprintf(stderr, "%s: too many arguments\n", args[0]);
        fprintf(stderr, "Try '%s --help' for more information\n",
        args[0]);
        return 1;
    }

    int babybird_parameter_count = 0;
    int eat_worms_parameter_count = 0;
    int fetch_worms_parameter_count = 0;
    int i;
    for(i = 1; i < nargs; i++){
    	if (strncmp(args[i], "--babies", 15) == 0 || strncmp(args[i], "-b", 15) == 0){
    		if(++babybird_parameter_count > 1){
    			fprintf(stderr, "%s: Multiple values for babies parameter.\n", args[0]);
    			fprintf(stderr, "Try %s: --help or -h for more information\n", args[0]);
               	return 1;
    		}
            num_baby_birds = atoi(args[++i]);
            if(num_baby_birds <= 0 || num_baby_birds > NPROC){
                fprintf(stderr, "%s: Please enter a valid value for number of baby birds.\n", args[0]);
                fprintf(stderr, "Try %s: --help or -h for more information\n", args[0]);
               	return 1;
            }                          
        }

        else if (strncmp(args[i], "--eat", 15) == 0 || strncmp(args[i], "-e", 15) == 0){
    		if(++eat_worms_parameter_count > 1){
    			fprintf(stderr, "%s: Multiple values for eat parameter.\n", args[0]);
    			fprintf(stderr, "Try %s: --help or -h for more information\n", args[0]);
               	return 1;
    		}
            num_eat_worms = atoi(args[++i]);
            if(num_eat_worms <= 0){
                fprintf(stderr, "%s: Please enter a valid value for eat worms.\n", args[0]);
                fprintf(stderr, "Try %s: --help or -h for more information\n", args[0]);
               	return 1;
            }                          
        }

        else if (strncmp(args[i], "--fetch", 15) == 0 || strncmp(args[i], "-f", 15) == 0){
    		if(++fetch_worms_parameter_count > 1){
    			fprintf(stderr, "%s: Multiple values for fetch parameter.\n", args[0]);
    			fprintf(stderr, "Try %s: --help or -h for more information\n", args[0]);
               	return 1;
    		}
            num_fetch_worms = atoi(args[++i]);
            if(num_fetch_worms <= 0){
                fprintf(stderr, "%s: Please enter a valid value for fetch worms.\n", args[0]);
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

    mutex_t* mutex;
    mutex_create(mutex);
    
    cond_t* fetch_mutex = &fm;
    cond_t* filldish = &fd;
    cond_init(fetch_mutex);
    cond_init(filldish);
    total_worms = 0;
    worm_count = 0;
    worms_on_dish = 0;


    resume(create(fetch_worms, 1024, 20, "parentbird", 3, mutex, fetch_mutex, filldish));
    char processName[10];
    for(i = 1; i <= num_baby_birds; i++){
        sprintf(processName, "babybird%d", i);
        resume(create(eat_worms, 1024, 20, processName, 4, i, mutex, fetch_mutex, filldish));
    }

    return 0;
}

void eat_worms(int bird_id, mutex_t* mutex, cond_t* fetch_mutex, cond_t* filldish){
    int i = 1;
    for(; i <= num_eat_worms; i++){
        mutex_lock(mutex);
        if(worms_on_dish == 0){
            cond_signal(fetch_mutex);
            cond_wait(filldish, mutex);
        }
        worms_on_dish--;
        worm_count++;
        printf("Baby bird %d ate a worm! (%d total)\n", bird_id, i);
        if(worm_count == num_baby_birds * num_eat_worms){
            printf("Worms left on the dish: %d\n", worms_on_dish);
        }
        mutex_unlock(mutex);
    }
}

void fetch_worms(mutex_t* mutex, cond_t* fetch_mutex, cond_t* filldish){
    while(total_worms < num_baby_birds * num_eat_worms){
        mutex_lock(mutex);
        cond_wait(fetch_mutex, mutex);
        worms_on_dish = num_fetch_worms;
        total_worms+=num_fetch_worms;
        printf("Parent bird filled the dish with %d worms!\n", num_fetch_worms);
        cond_signal(filldish);
        mutex_unlock(mutex);
    }
}