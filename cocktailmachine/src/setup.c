#include "../include/setup.h"
#include "SEGGER_RTT_printf.c"
int debug = 1;

// Basic Variables
int ready = 0;

// Semaphores
K_SEM_DEFINE(move_to_pos_sem, 0, 1);
K_SEM_DEFINE(fill_glass_sem, 0, 1);

// Queue
K_QUEUE_DEFINE(position_q);
K_QUEUE_DEFINE(amount_q);

// GPIOS
const struct gpio_dt_spec dir_hor_spec = GPIO_DT_SPEC_GET(DT_NODELABEL(dir_hor), gpios);
const struct gpio_dt_spec step_hor_spec = GPIO_DT_SPEC_GET(DT_NODELABEL(step_hor), gpios);
const struct gpio_dt_spec dir_ver_spec = GPIO_DT_SPEC_GET(DT_NODELABEL(dir_ver), gpios);
const struct gpio_dt_spec step_ver_spec = GPIO_DT_SPEC_GET(DT_NODELABEL(step_ver), gpios);


const struct gpio_dt_spec limit_sw_hor0_spec = GPIO_DT_SPEC_GET(DT_NODELABEL(limit_sw_hor0), gpios);
const struct gpio_dt_spec limit_sw_hor1_spec = GPIO_DT_SPEC_GET(DT_NODELABEL(limit_sw_hor1), gpios);
const struct gpio_dt_spec limit_sw_ver0_spec = GPIO_DT_SPEC_GET(DT_NODELABEL(limit_sw_ver0), gpios);
const struct gpio_dt_spec limit_sw_ver1_spec = GPIO_DT_SPEC_GET(DT_NODELABEL(limit_sw_ver1), gpios);


void setup(void){

    gpio_pin_configure_dt(&dir_hor_spec, GPIO_OUTPUT_INACTIVE);
    gpio_pin_configure_dt(&step_hor_spec, GPIO_OUTPUT_INACTIVE);
    gpio_pin_configure_dt(&dir_ver_spec, GPIO_OUTPUT_INACTIVE);
    gpio_pin_configure_dt(&step_ver_spec, GPIO_OUTPUT_INACTIVE);

    gpio_pin_configure_dt(&limit_sw_hor0_spec, GPIO_INPUT);
    gpio_pin_configure_dt(&limit_sw_hor1_spec, GPIO_INPUT);
    gpio_pin_configure_dt(&limit_sw_ver0_spec, GPIO_INPUT);
    gpio_pin_configure_dt(&limit_sw_ver1_spec, GPIO_INPUT);

    gpio_pin_set_dt(&dir_hor_spec, DIR_HOR_RIGHT);
    gpio_pin_set_dt(&dir_ver_spec, DIR_VER_DOWN);

}

