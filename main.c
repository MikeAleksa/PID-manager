#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "pid_manager.h"

// This is a driver program to test the pid_manager

int main() {

    srand(time(NULL));

    int map_allocated = allocate_map();
    if (map_allocated == -1)
        return (1);

    for (int i = 0; i <= 4810; i++) {
        int pid = allocate_pid();
        if (pid == -1)
            printf("allocation failed: no pids available\n");
        else
            printf("allocated pid %i\n", pid);
    }

    for (int j = 0; j < 100; j++) {
        for (int i = 0; i < 10; i++) {
            int pid = rand() % 4700 + 300;
            release_pid(pid);
            printf("release pid %i\n", pid);
        }

        for (int i = 0; i < 10; i++) {
            int pid = allocate_pid();
            printf("allocated pid %i\n", pid);
        }
    }

    deallocate_map();
    return 0;
}