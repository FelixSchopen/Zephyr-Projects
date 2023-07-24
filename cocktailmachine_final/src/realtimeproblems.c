#include "../include/realtimeproblems.h"

K_MUTEX_DEFINE(resource_mutex1);
K_MUTEX_DEFINE(resource_mutex2);

K_SEM_DEFINE(inversion_sem, 0, 1);
K_SEM_DEFINE(deadlock_sem, 0, 1);

int release_deadlock = 0;

// Locks two mutexes and unlocks them again
// This function is called every time one of the motors takes a step
// This function does esentially nothing. It is only used to provoce a deadlock and priority inversion
void access_virtual_resource_1(void){
    // Try to lock mutex1
    while(k_mutex_lock(&resource_mutex1, K_NO_WAIT) != 0) {
        k_msleep(100);        
    }

    k_usleep(1); // Put thread to sleep, in order to increase deadlock propability

    // Try to lock mutex2
    while(k_mutex_lock(&resource_mutex2, K_NO_WAIT) !=0){
        k_msleep(100);
    }
    
    // If the system had a shared resource it should be acessed here
    //
    //
    
    // Unlock mutexes
    k_mutex_unlock(&resource_mutex1);
    k_mutex_unlock(&resource_mutex2);
}

// Locks two mutexes in different order and unlocks them again
// This function is called repeatedly when a deadlock should be triggered.
// Unlocking the mutexes in different orders will trigger a deadlock
int access_virtual_resource_2(void){
    // Try to lock mutex2
    while(k_mutex_lock(&resource_mutex2, K_NO_WAIT) != 0) {
        k_msleep(100);
    } 
    
    k_usleep(1); // Put thread to sleep, in order to increase deadlock propability

    // Try to lock mutex1
    while(k_mutex_lock(&resource_mutex1, K_NO_WAIT) != 0){
        if(release_deadlock){
            return -1;
        }
        k_msleep(100);
    }
    
    // If the system had a shared resource it should be acessed here
    //
    //

    // Unlock mutexes
    k_mutex_unlock(&resource_mutex2);
    k_mutex_unlock(&resource_mutex1);
}

// Blocks one of the mutexes that the motor needs in order to take one step 
// Unblocks the mutex after 5 seconds
void block_mutex_inversion(void){
    k_mutex_lock(&resource_mutex1, K_FOREVER);
    k_msleep(5000);
    k_mutex_unlock(&resource_mutex1);
}
