#include "../include/realtimeproblems.h"

K_MUTEX_DEFINE(resource_mutex1);
K_MUTEX_DEFINE(resource_mutex2);

K_SEM_DEFINE(inversion_sem, 0, 1);
K_SEM_DEFINE(deadlock_sem, 0, 1);

int release_deadlock = 0;

void access_shared_resources_1(void){
    // Try to lock mutex1
    while(k_mutex_lock(&resource_mutex1, K_NO_WAIT) != 0) {
        k_usleep(1);        
    }

    k_usleep(1); // Put thread to sleep, in order to increase deadlock propability

    // Try to lock mutex2
    while(k_mutex_lock(&resource_mutex2, K_NO_WAIT) !=0){
        k_usleep(1);
    }
    
    // Resource should be acessed here
    //
    //
    
    // Unlock mutexes
    k_mutex_unlock(&resource_mutex1);
    k_mutex_unlock(&resource_mutex2);
}


int access_shared_resources_2(void){
    // Try to lock mutex2
    while(k_mutex_lock(&resource_mutex2, K_NO_WAIT) != 0) {
        if(release_deadlock){
            return -1;
        }
        k_usleep(1);
    } 
    
    k_usleep(1); // Put thread to sleep, in order to increase deadlock propability

    // Try to lock mutex1
    while(k_mutex_lock(&resource_mutex1, K_NO_WAIT) != 0){
        if(release_deadlock){
            return -1;
        }
        k_usleep(1);
    }
    
    // Resource should be acessed here
    //
    //

    // Unlock mutexes
    k_mutex_unlock(&resource_mutex2);
    k_mutex_unlock(&resource_mutex1);
}

void block_mutex_inversion(void){
    k_mutex_lock(&resource_mutex1, K_FOREVER);
    k_msleep(5000);
    k_mutex_unlock(&resource_mutex1);
}
