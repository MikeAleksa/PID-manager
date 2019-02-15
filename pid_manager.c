#include <stdlib.h>         /* for malloc() */
#include <limits.h>         /* for CHAR_BIT */
#include "pid_manager.h"

#define MIN_PID 300
#define MAX_PID 5001
const unsigned char fullWord = ~((unsigned char) 0);     /* used to check for full words in the bitmap */
unsigned char *pid_map = NULL;

/* allocate a bitmap using an array of unsigned chars and assign the bitmap to the global pointer 'pid_map'
 * return values are 1 (successful allocation) and -1 (unsuccessful allocation)
 *
 * this bitmap allocates space for pids below MIN_PID under the assumption they will be used as
 * pids reserved for special processes, similar to the RESERVED_PIDS in the linux kernel's pid.c */
int allocate_map(void) {
    // if MAX_PID does not divide evenly by CHAR_BIT, include one extra word in pid_map
    unsigned char extraWord = 0;
    if (MAX_PID % CHAR_BIT != 0)
        extraWord = 1;
    // allocate a pid_map of unsigned char * and return 0 if allocation fails, or set all words
    //  in pid_map to 0 and return 1
    pid_map = (unsigned char *) malloc(MAX_PID / CHAR_BIT + extraWord);
    if (pid_map == NULL) return -1;
    for (int i = 0; i < MAX_PID / CHAR_BIT; i++) {
        pid_map[i] = 0;
    }
    return 1;
}

/* search for first free pid between MIN_PID and MAX_PID and allocate that pid
 * returns the pid number, or will return -1 if all pids are already allocated */
int allocate_pid(void) {
    // search for first word in the pid_map that is not full
    for (int word = MIN_PID / CHAR_BIT; word < MAX_PID / CHAR_BIT; word++) {
        // handle word containing MIN_PID if it contains both reserved and unreserved pids
        //  by checking the correspond bits >= MIN_PID
        if (word == MIN_PID / CHAR_BIT && MIN_PID % CHAR_BIT != 0) {
            for (int bit = MIN_PID % CHAR_BIT; bit < CHAR_BIT; bit++) {
                if (pid_map[word] & (1u << bit)) continue;
                pid_map[word] |= (1u << bit);
                return (word * CHAR_BIT) + bit;
            }
        }
            // handle all other words that contain only non-reserved pids by checking checking
            //  individual bits only if the bits in the word are not all 1s
        else if (pid_map[word] != fullWord) {
            for (int bit = 0; bit < CHAR_BIT; bit++) {
                // if statement checks if (a bit in the pid_map & 1) is false, i.e. the bit in pid_map = 0
                if (pid_map[word] & (1u << bit)) continue;
                pid_map[word] |= (1u << bit);
                return (word * CHAR_BIT) + bit; // pid numbers index from
            }
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
