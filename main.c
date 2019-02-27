/* Written by Michael Aleksa for CISC-3320 EM6 */

#include <stdio.h>          /* for printf() */
#include <time.h>           /* for time() to seed srand() */
#include <stdlib.h>         /* for rand() */
#include <unistd.h>         /* for sleep() */
#include "pid_manager.h"

void create_threads();

void simulate_process();

int main() {
    srand(time(NULL));

    /* allocate pid map */
    int map_allocated = allocate_map();
    if (map_allocated == -1) {
        printf("pid_map failed to allocate (-1)");
        exit(-1);
    }

    /* create 100 threads */
    create_threads();

    /* simulate processes on threads */


    /* deallocate pid map */
    deallocate_map();
    return 0;
}


void create_threads() {

}

void simulate_process() {
    /* try to allocate a pid and return if unable */
    int pid = allocate_pid();
    if (pid == -1) {
        printf("allocation failed: no pids available\n");
        return;
    } else {
        printf("allocated pid %i\n", pid);
    }
    /* sleep for a random amount of time between 5 and 30 seconds */
    unsigned int sleep_time = 5u + (rand() % 25u);
    sleep(sleep_time);
    /* release pid */
    release_pid(pid);
    printf("release pid %i\n", pid);
}