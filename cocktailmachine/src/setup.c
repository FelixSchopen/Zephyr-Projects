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
const struct gpio_dt_spec dir_spec = GPIO_DT_SPEC_GET(DT_NODELABEL(dir), gpios);
const struct gpio_dt_spec step_spec = GPIO_DT_SPEC_GET(DT_NODELABEL(step), gpios);

void setup(void){

    gpio_pin_configure_dt(&dir_spec, GPIO_OUTPUT_INACTIVE);
    gpio_pin_configure_dt(&step_spec, GPIO_OUTPUT_INACTIVE);

    gpio_pin_set_dt(&dir_spec, 0);

}

