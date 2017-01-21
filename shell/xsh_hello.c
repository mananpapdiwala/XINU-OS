/* xsh_date.c - xsh_date */

#include <xinu.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

/*------------------------------------------------------------------------
 * xsh_hello - Print a welcome statement by greeting the string passes as input.
 *------------------------------------------------------------------------
 */
shellcmd xsh_hello(int nargs, char *args[]) {
        /* Output info for '--help' argument */

        if (nargs == 2 && strncmp(args[1], "--help", 7) == 0) {
                printf("Usage: %s\n\n", args[0]);
                printf("Description:\n");
                printf("\tAccepts one string argument\n");
                printf("\tThe argument cannot be empty or contain only whitespaces\n");
                printf("\tDisplays a standard welcome message by greeting the string passed as argument\n");
                return 0;
        }

        /* Check argument count */

        if (nargs > 2) {
                fprintf(stderr, "%s: too many arguments\n", args[0]);
                fprintf(stderr, "Try '%s --help' for more information\n",
                        args[0]);
                return 1;
        }

        if (nargs < 2) {
                fprintf(stderr, "%s: few arguments\n", args[0]);
                fprintf(stderr, "Try '%s --help' for more information\n", args[0]);
                return 1;
        }

        /*if (nargs == 2) {
                int isWhiteSpace = 0;
                while (*args[1]!='\0'){
                        if (isspace(*args[1])){
                                isWhiteSpace = 1;
                                break;
                        }
                        args[1]++;
                }
                if (isWhiteSpace == 1){
                        fprintf(stderr, "%s: No whitespaces allowed\n", args[0]);
                        fprintf(stderr, "Try '%s --help' for more information\n", args[0]);
                        return 1;
                }
        }*/
        printf("Hello %s, Welcome to the world of Xinu!!\n", args[1]);
        return 0;
}
