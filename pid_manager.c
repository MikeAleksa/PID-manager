#include <stdlib.h>         /* for calloc() */
#include <limits.h>         /* for CHAR_BIT */
#include "pid_manager.h"

unsigned char *pid_map = NULL;                          /* to hold position of map allocated in allocate_map */
size_t words_in_map;
const unsigned char fullWord = ~((unsigned char) 0);    /* to check for full words in the bitmap */

/* allocate a bitmap using an array of unsigned chars and assign the bitmap to the global pointer 'pid_map'
 * return values are 1 (successful allocation) and -1 (unsuccessful allocation)
 *  note: this bitmap allocates space for pids below MIN_PID under the assumption they will be used as
 *  pids reserved for special processes, similar to the RESERVED_PIDS in the linux kernel's pid.c */
int allocate_map(void) {
    // allocate an additional word if the total pids do not divide evenly by the pid_map word size
    words_in_map = (MAX_PID + 1) / CHAR_BIT;
    if ((MAX_PID + 1) % CHAR_BIT != 0) {
        words_in_map += 1;
    }
    // allocate a pid_map of unsigned char * and return -1 if allocation fails, or 1 if allocation succeeds
    // the bitmap represents pids from 0 to MAX_PID
    //  note: calloc() initializes all values to 0
    pid_map = (unsigned char *) calloc(words_in_map, sizeof(char));
    if (pid_map == NULL) return -1;
    else return 1;
}

/* search for first free pid between MIN_PID and MAX_PID and allocate that pid
 * returns the pid number, or will return -1 if all pids are already allocated */
int allocate_pid1(void) {
    // search for first word in the pid_map that is not full
    for (int word = MIN_PID / CHAR_BIT; word < words_in_map; word++) {
        if (pid_map[word] != fullWord) {
            for (int bit = 0; bit < CHAR_BIT; bit++) {
                // if statement does nothing if (a bit in the pid_map & 1) is true, i.e. the bit in pid_map = 1
                if (pid_map[word] & (1u << bit)) continue;
                pid_map[word] |= (1u << bit);
                return (word * CHAR_BIT) + bit;
            }
        }
    }
    return -1;
}

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

/* Release a pid by setting it's associated bit in pid_map to 0 */
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
