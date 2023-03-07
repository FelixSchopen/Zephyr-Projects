#ifndef RTPROBLEMS
#define RTPROBLEMS

#include "setup.h"

void access_shared_resource1(void);
void access_shared_resource2(void);

extern struct k_mutex my_mutex1;
extern struct k_mutex my_mutex2;
extern struct k_mutex block_mutex;

extern struct k_sem resource_sem;



extern int deadlock;
extern int resource;


#endif //RTPROBLEMS
