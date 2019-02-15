#include <stdlib.h>         /* for calloc() */
#include <limits.h>         /* for CHAR_BIT */
#include "pid_manager.h"

#define MIN_PID 300
#define MAX_PID 5000
unsigned char *pid_map = NULL;

/* allocate a bitmap using an array of unsigned chars and assign the bitmap to the global pointer 'pid_map'
 * return values are 1 (successful allocation) and -1 (unsuccessful allocation)
 *  note: this bitmap allocates space for pids below MIN_PID under the assumption they will be used as
 *  pids reserved for special processes, similar to the RESERVED_PIDS in the linux kernel's pid.c */
int allocate_map(void) {
    // if MAX_PID does not divide evenly by CHAR_BIT, include one extra word in pid_map for the remainder
    size_t words = MAX_PID / CHAR_BIT;
    if (MAX_PID % CHAR_BIT != 0) {
        words += 1;
    }
    // allocate a pid_map of unsigned char * and return -1 if allocation fails or 1 if allocation succeeds
    //  note: calloc() initializes all values to 0
    pid_map = (unsigned char *) calloc(words, sizeof(char));
    if (pid_map == NULL) return -1;
    else return 1;
}

/* search for first free pid between MIN_PID and MAX_PID and allocate that pid
 * returns the pid number, or will return -1 if all pids are already allocated */
int allocate_pid(void) {
    // search for first free pid between MIN_PID and MAX_PID and allocate
    for (int pid = MIN_PID; pid <= MAX_PID; pid++) {
        // do nothing if (a pid in the pid_map & 1) is true, i.e. the pid in pid_map = 1
        // else, allocate and return pid
        if (pid_map[pid / CHAR_BIT] & (1 << (pid % CHAR_BIT)))
            continue;
        else {
            pid_map[pid / CHAR_BIT] |= (1 << (pid % CHAR_BIT));
            return pid;
        }
    }
    return -1;
}

/* Release a pid by setting it's associated bit in pid_map to 0 using bitwise operators */
void release_pid(int pid) {
    // don't allow pids outside MIN_PID and MAX_PID to be released
    if (pid < MIN_PID || pid > MAX_PID)
        return;
    pid_map[pid / CHAR_BIT] &= ~(1u << (pid % CHAR_BIT));
}

/* deallocate the bitmap assigned to pid_map */
void deallocate_map(void) {
    free(pid_map);
    pid_map = NULL;
}
