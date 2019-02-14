#ifndef PID_MANAGER_H
#define PID_MANAGER_H

int allocate_map(void);
int allocate_pid(void);
void release_pid(int pid);
void deallocate_map(void);

#endif //PID_MANAGER_H
