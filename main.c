#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "pid_manager.h"

// This is a driver program to test the pid_manager program

int main() {

    srand(time(NULL));

    int map_allocated = allocate_map();
    if (map_allocated == -1)
        return (-1);

    // allocate all available pids with no print statements
    for (int i = 0; i <= 5001; i++) {
        int pid = allocate_pid();
        if (pid == -1)
            printf("allocation failed: no pids available\n");
        else
            printf("allocated pid %i\n", pid);
    }

    // show release_pid and allocate_pid working with print statements
    for (int j = 0; j < 3; j++) {
        // release pid between 300 and 5000
        for (int i = 0; i < 5; i++) {
            int pid = rand() % 4700 + 300;
            release_pid(pid);
            printf("release pid %i\n", pid);
        }

        // try to allocate one more pid than available
        for (int i = 0; i < 6; i++) {
            int pid = allocate_pid();
            if (pid == -1)
                printf("allocation failed: no pids available\n");
            else
                printf("allocated pid %i\n", pid);
        }
    }

    deallocate_map();
    return 0;
}