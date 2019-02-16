#include <stdlib.h>         /* for calloc() */
#include <limits.h>         /* for CHAR_BIT */
#include "pid_manager.h"

unsigned char *pid_map = NULL;                          /* to hold position of map allocated in allocate_map */
size_t words_in_map;                                    /* to hold number of words (unsigned char *) in pid_map */
const unsigned char fullWord = ~((unsigned char) 0);    /* to check for full words in the bitmap */

/* allocate a bitmap representing pids from 0 to MAX_PID
 * return values are 1 (successful allocation) and -1 (unsuccessful allocation)
 *  note: this bitmap allocates space for pids below MIN_PID under the assumption they will be used as
 *  pids reserved for special processes, similar to the RESERVED_PIDS in the linux kernel's pid.c */
int allocate_map(void) {
    /* calculate ceiling of MAX_PID / CHAR_BIT */
    words_in_map = (MAX_PID + 1) / CHAR_BIT;
    if ((MAX_PID + 1) % CHAR_BIT != 0) {
        words_in_map += 1;
    }
    /* allocate the pid_map and return -1 if allocation fails, or 1 if allocation succeeds
     *  note: calloc() initializes all values to 0 */
    pid_map = (unsigned char *) calloc(words_in_map, sizeof(char));
    if (pid_map == NULL) return -1;
    else return 1;
}

/* search for first free pid between MIN_PID and MAX_PID and allocate
 * returns the pid number, or  -1 if all pids are already allocated
 *  note: checking whole words at a time is faster than checking each bits sequentially */
int allocate_pid(void) {
    /* search for first word in the pid_map that is not full */
    for (int word = MIN_PID / CHAR_BIT; word < words_in_map; word++) {
        if (pid_map[word] != fullWord) {
            /* for each bit in the word, allocate and assign pid available and within assignable range */
            for (int bit = 0; bit < CHAR_BIT; bit++) {
                int pid = word * CHAR_BIT + bit;
                if (pid_map[word] & (1u << bit) || pid < MIN_PID || pid > MAX_PID)
                    continue;
                else {
                    pid_map[word] |= (1u << bit);
                    return pid;
                }
            }
        }
    }
    return -1; /* return -1 if no available pids were found */
}

/* Release a pid by setting it's associated bit in pid_map to 0 */
void release_pid(int pid) {
    /* ignore pids outside allowable range */
    if (pid < MIN_PID || pid > MAX_PID)
        return;
    pid_map[pid / CHAR_BIT] &= ~(1u << (pid % CHAR_BIT));
}

/* deallocate the bitmap assigned to pid_map */
void deallocate_map(void) {
    free(pid_map);
    pid_map = NULL;
}
