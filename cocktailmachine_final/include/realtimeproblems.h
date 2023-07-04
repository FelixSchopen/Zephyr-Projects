#ifndef RTPROBLEMS
#define RTPROBLEMS

#include "setup.h"

void access_shared_resources_1(void);
int access_shared_resources_2(void);

void block_mutex_inversion(void);


extern struct k_mutex resource_mutex1;
extern struct k_mutex resource_mutex2;

extern struct k_sem inversion_sem;
extern struct k_sem deadlock_sem;

extern int release_deadlock;

#endif //RTPROBLEMS
