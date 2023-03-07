#include "../include/realtimeproblems.h"

K_MUTEX_DEFINE(my_mutex1);
K_MUTEX_DEFINE(my_mutex2);
K_MUTEX_DEFINE(block_mutex);

K_SEM_DEFINE(resource_sem, 0, 1);


int deadlock = 0;
int resource = 0;


void access_shared_resource1(void){
    // Try to lock mutex1
    while(k_mutex_lock(&my_mutex1, K_NO_WAIT) != 0) {
        k_usleep(0.9);
        if(deadlock == 0){
            return;
        }
    }
    k_usleep(1);    
    // Try to lock mutex2
    while(k_mutex_lock(&my_mutex2, K_NO_WAIT) !=0){
        k_usleep(0.9);
        if(deadlock == 0){
            return;
        }
    }
    
    // Access ressource here
    
    // Unlock mutexes
    k_mutex_unlock(&my_mutex1);
    k_mutex_unlock(&my_mutex2);
}


void access_shared_resource2(void){
    // Try to lock mutex1
    while(k_mutex_lock(&my_mutex2, K_NO_WAIT) != 0) {
        k_usleep(0.9);
        if(deadlock == 0){
            return;
        }
    } 
    k_usleep(0.9);  
    // Try to lock mutex2
    while(k_mutex_lock(&my_mutex1, K_NO_WAIT) != 0){
        k_usleep(0.9);
        if(deadlock == 0){
            return;
        }
    }
    
    // Access ressource here

    // Unlock mutexes
    k_mutex_unlock(&my_mutex2);
    k_mutex_unlock(&my_mutex1);
}
