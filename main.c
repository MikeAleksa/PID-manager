/* Written by Michael Aleksa for CISC-3320 EM6 */

#include <stdio.h>          /* for printf() */
#include <time.h>           /* for time() to seed srand() */
#include <stdlib.h>         /* for rand() */
#include <unistd.h>         /* for sleep() */
#include <pthread.h>        /* for pthreads */
#include "pid_manager.h"

#define NUM_THREADS 100
pthread_mutex_t lock;

void *simulate_process_runnable();

int main() {
    srand(time(NULL));

    /* create array of thread ids and initialize mutex */
    pthread_t tid[NUM_THREADS];
    pthread_mutex_init(&lock, NULL);

    /* allocate pid map */
    int map_allocated = allocate_map();
    if (map_allocated == -1) {
        printf("pid_map failed to allocate (-1)");
        exit(-1);
    }

    /* create 100 threads and simulate processes on threads*/
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_create(&tid[i], NULL, simulate_process_runnable, NULL);
    }

    /* wait until all threads are finished */
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(tid[i], NULL);
    }

    /* deallocate pid map and destory mutex */
    deallocate_map();
    pthread_mutex_destroy(&lock);
    return 0;
}

void *simulate_process_runnable() {
    /* try to allocate a pid and return if unable
     * use mutex to block other threads while a thread is allocating a pid,
     * so two threads cannot allocate the same pid */
    pthread_mutex_lock(&lock);
    int pid = allocate_pid();
    pthread_mutex_unlock(&lock);
    if (pid == -1) {
        printf("allocation failed: no pids available\n");
        pthread_exit(0);
    } else {
        printf("allocated pid %i\n", pid);
    }
    /* sleep for a random amount of time between 0 and 5 seconds */
    unsigned int sleep_time = 0u + (rand() % 5u);
    sleep(sleep_time);
    /* release pid */
    release_pid(pid);
    printf("released pid %i\n", pid);
    pthread_exit(0);
}