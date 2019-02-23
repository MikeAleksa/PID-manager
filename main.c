/* Written by Michael Aleksa for CISC-3320 EM6 */

#include <stdio.h>        /* for printf() */
#include <time.h>         /* for time() to seed srand() */
#include <stdlib.h>       /* for rand() */
#include "pid_manager.h"

/* This is a driver program to test the pid_manager program */

int main() {
    srand(time(NULL));

    int map_allocated = allocate_map();
    if (map_allocated == -1) {
        printf("pid_map failed to allocate (-1)");
        return (-1);
    }

    printf("allocating pids from %i to %i...\n", MIN_PID, MAX_PID);
    /* allocate all available pids with no print statements */
    for (int i = MIN_PID; i <= MAX_PID; i++) {
        allocate_pid();
    }
    printf("all pids allocated\n\n");

    /* show release_pid and allocate_pid working with print statements */
    for (int j = 0; j < 3; j++) {
        for (int i = 0; i < 5; i++) {
            /* release a random pid between MIN_PID and MAX_PID */
            int pid = MIN_PID + rand() % (MAX_PID + 1 - MIN_PID);
            release_pid(pid);
            printf("release pid %i\n", pid);
        }
        printf("\n");

        for (int i = 0; i < 6; i++) {
            /* try to allocate a pid */
            int pid = allocate_pid();
            if (pid == -1)
                printf("allocation failed: no pids available\n");
            else
                printf("allocated pid %i\n", pid);
        }
        printf("\n");
    }

    deallocate_map();
    return 0;
}