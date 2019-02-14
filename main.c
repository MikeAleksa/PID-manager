#include <stdio.h>
#include "pid_manager.h"

// This is a driver program to test the pid_manager

int main() {
    int map_allocated = allocate_map();
    if (map_allocated == -1)
        return (1);

    for (int i = 0; i <= 4701; i++) {
        int pid = allocate_pid();
        if (pid == -1)
            printf("allocation failed: no pids available\n");
        else
            printf("allocated pid %i\n", pid);
    }

    release_pid(3798);
    int pid = allocate_pid();
    printf("allocated pid %i\n", pid);

    deallocate_map();
    return 0;
}