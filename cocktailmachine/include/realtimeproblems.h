#ifndef RTPROBLEMS
#define RTPROBLEMS

#include "setup.h"

void access_virtual_resource_1(void);
void access_shared_resources_2(void);

void block_resource(void);

extern struct k_mutex resource_mutex1;
extern struct k_mutex resource_mutex2;
extern struct k_mutex block_mutex;

extern struct k_sem inversion_sem;



extern int release_deadlock;
extern int resource;


#endif //RTPROBLEMS
