#include "../include/setup.h"
#include "SEGGER_RTT_printf.c"

// Basic Variables
int ready = 0;

// Semaphores
K_SEM_DEFINE(move_to_pos_sem, 0, 1);
K_SEM_DEFINE(fill_glass_sem, 0, 1);

// Queue
K_QUEUE_DEFINE(position_q);

// GPIOS

void setup(void){

}

