/* Written by Michael Aleksa for CISC-3320 EM6 */

#include <stdio.h>          /* for printf() */
#include <time.h>           /* for time() */
#include <stdlib.h>         /* for rand() */
#include <unistd.h>         /* for sleep() */
#include <pthread.h>        /* for pthreads */
#include "pid_manager.h"

#define NUM_THREADS 100
pthread_mutex_t lock;

void *simulate_process_runnable();

int main() {
    srand(time(NULL));

    /* create an array of pthread ids and initialize mutex */
    pthread_t tid[NUM_THREADS];
    if (pthread_mutex_init(&lock, NULL) != 0) {
        printf("unable to intialize mutex\n");
        return 0;
    }

    /* allocate pid map */
    int map_allocated = allocate_map();
    if (map_allocated == -1) {
        printf("pid_map failed to allocate (-1)");
        exit(-1);
    }

    /* create 100 threads and simulate processes running on threads */
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_create(&tid[i], NULL, simulate_process_runnable, NULL);
    }

    /* wait until all threads are finished running */
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(tid[i], NULL);
    }

    /* deallocate pid map and destroy mutex */
    deallocate_map();
    pthread_mutex_destroy(&lock);
    return 0;
}

/* simulate processes running by allocating pids, waiting a random amount of time, and then freeing the pid
 *
 * a mutex is used to block other threads when a thread is interacting with the pid map
 *  this avoids race conditions that might cause a pid to be allocated twice, or to print that it
 *  was re-allocated after being released before printing that the pid was released */
void *simulate_process_runnable() {
    /* try to allocate a pid and return if unable */
    pthread_mutex_lock(&lock);
    int pid = allocate_pid();
    if (pid == -1) {
        printf("allocation failed: no pids available\n");
        pthread_exit(0);
    } else {
        printf("allocated pid %i\n", pid);
    }
    pthread_mutex_unlock(&lock);

    /* sleep for a random amount of time between 0 and 5 seconds */
    sleep(rand() % 5u);

    /* release pid */
    pthread_mutex_lock(&lock);
    release_pid(pid);
    printf("released pid %i\n", pid);
    pthread_mutex_unlock(&lock);
    pthread_exit(0);
}