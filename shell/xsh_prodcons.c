/*xsh_prodcons.c - xsh_prodcons*/
#include <xinu.h>
#include <futures.h>

/*------------------------------------------------------------------------
 * xsh_prodcons - Implementation of futures on producer and consumer.
 *------------------------------------------------------------------------
 */

shellcmd xsh_prodcons(int nargs, char *args[]){
 	if(nargs < 2){
    	fprintf(stderr, "%s: few arguments\n", args[0]);
        fprintf(stderr, "Try '%s --help' for more information\n", args[0]);
        return 1;
    }
    if(nargs > 2){
    	fprintf(stderr, "%s: too many arguments\n", args[0]);
        fprintf(stderr, "Try '%s --help' for more information\n",
        args[0]);
        return 1;
    }
 	if (nargs == 2 && (strncmp(args[1], "--help", 7) == 0 || strncmp(args[1], "-h", 7) == 0)) {
                printf("Usage: %s\n\n", args[0]);
               	printf("Implements futures on producer and consumer problem.\n");
                printf("Description:\n");
                printf("\t\n");
                printf("\tOptions:\n");
                printf("\t-f \tCreates a future\n");
                printf("\t-h or --help \tDisplays this help and exit\n");

                return 0;
    }

    

    if(nargs == 2 && strncmp(args[1], "-f", 7) == 0){
    	future_t* f_exclusive, * f_shared, * f_queue, * f_test;

  		f_exclusive = future_alloc(FUTURE_EXCLUSIVE);
  		f_shared    = future_alloc(FUTURE_SHARED);
  		f_queue     = future_alloc(FUTURE_QUEUE);
  		resume( create(future_cons, 1024, 20, "fcons1", 1, f_exclusive) );
  		resume( create(future_prod, 1024, 20, "fprod1", 2, f_exclusive, 1) );
  		
  		resume( create(future_cons, 1024, 20, "fcons2", 1, f_shared) );
  		resume( create(future_cons, 1024, 20, "fcons3", 1, f_shared) );
  		resume( create(future_cons, 1024, 20, "fcons4", 1, f_shared) ); 
  		resume( create(future_cons, 1024, 20, "fcons5", 1, f_shared) );
  		resume( create(future_prod, 1024, 20, "fprod2", 2, f_shared, 2) );

  		resume( create(future_cons, 1024, 20, "fcons6", 1, f_queue) );
  		resume( create(future_cons, 1024, 20, "fcons7", 1, f_queue) );
  		resume( create(future_cons, 1024, 20, "fcons8", 1, f_queue) );
  		resume( create(future_cons, 1024, 20, "fcons9", 1, f_queue) );
  		resume( create(future_prod, 1024, 20, "fprod3", 2, f_queue, 3) );
  		resume( create(future_prod, 1024, 20, "fprod4", 2, f_queue, 4) );
  		resume( create(future_prod, 1024, 20, "fprod5", 2, f_queue, 5) );
  		resume( create(future_prod, 1024, 20, "fprod6", 2, f_queue, 6) );
  		
  		//future_free(f_exclusive);
  		//future_free(f_shared);
  		//future_free(f_queue);

  		return 0;
 
    }

    fprintf(stderr, "%s: Invalid arguments\n", args[0]);
    fprintf(stderr, "Try '%s --help' for more information\n", args[0]);
    return 1;
}