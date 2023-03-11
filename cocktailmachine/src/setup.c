#include "../include/setup.h"
#include "SEGGER_RTT_printf.c"
int debug = 1;

// Basic Variables
int ready = 0;

// Semaphores
K_SEM_DEFINE(move_to_pos_sem, 0, 1);
K_SEM_DEFINE(fill_glass_sem, 0, 1);
K_SEM_DEFINE(drink_sem, 0, 1);
K_SEM_DEFINE(cocktail_sem, 0, 1);
K_SEM_DEFINE(init_ver, 0, 1);
K_SEM_DEFINE(init_hor, 0, 1);

// Queue
K_QUEUE_DEFINE(position_q);
K_QUEUE_DEFINE(amount_q);

// GPIOS
const struct gpio_dt_spec ver_step_spec = GPIO_DT_SPEC_GET(DT_NODELABEL(ver_step), gpios);
const struct gpio_dt_spec ver_dir_spec = GPIO_DT_SPEC_GET(DT_NODELABEL(ver_dir), gpios);
const struct gpio_dt_spec ver_enable_spec = GPIO_DT_SPEC_GET(DT_NODELABEL(ver_enable), gpios);

const struct gpio_dt_spec hor_step_spec = GPIO_DT_SPEC_GET(DT_NODELABEL(hor_step), gpios);
const struct gpio_dt_spec hor_dir_spec = GPIO_DT_SPEC_GET(DT_NODELABEL(hor_dir), gpios);
const struct gpio_dt_spec hor_enable_spec = GPIO_DT_SPEC_GET(DT_NODELABEL(hor_enable), gpios);

const struct gpio_dt_spec ver_limit_sw0_spec = GPIO_DT_SPEC_GET(DT_NODELABEL(ver_limit_sw0), gpios);
const struct gpio_dt_spec hor_limit_sw0_spec = GPIO_DT_SPEC_GET(DT_NODELABEL(hor_limit_sw0), gpios);
const struct gpio_dt_spec user_button_spec = GPIO_DT_SPEC_GET(DT_NODELABEL(user_button), gpios);




// Status LEDs
const struct gpio_dt_spec green = GPIO_DT_SPEC_GET(DT_NODELABEL(green_led_4), gpios);
const struct gpio_dt_spec red = GPIO_DT_SPEC_GET(DT_NODELABEL(red_led_5), gpios);

const struct gpio_dt_spec blue = GPIO_DT_SPEC_GET(DT_NODELABEL(blue_led_6), gpios);
const struct gpio_dt_spec orange = GPIO_DT_SPEC_GET(DT_NODELABEL(orange_led_3), gpios);

void setup(void){

    gpio_pin_configure_dt(&hor_dir_spec, GPIO_OUTPUT_INACTIVE);
    gpio_pin_configure_dt(&hor_step_spec, GPIO_OUTPUT_INACTIVE);
    gpio_pin_configure_dt(&hor_enable_spec, GPIO_OUTPUT_ACTIVE);

    gpio_pin_configure_dt(&ver_dir_spec, GPIO_OUTPUT_INACTIVE);
    gpio_pin_configure_dt(&ver_step_spec, GPIO_OUTPUT_INACTIVE);
    gpio_pin_configure_dt(&ver_enable_spec, GPIO_OUTPUT_ACTIVE);


    gpio_pin_configure_dt(&hor_limit_sw0_spec, GPIO_INPUT);
    gpio_pin_configure_dt(&ver_limit_sw0_spec, GPIO_INPUT);
    gpio_pin_configure_dt(&user_button_spec, GPIO_INPUT);


    gpio_pin_configure_dt(&green, GPIO_OUTPUT_INACTIVE);    
    gpio_pin_configure_dt(&red, GPIO_OUTPUT_INACTIVE);
    
    gpio_pin_configure_dt(&blue, GPIO_OUTPUT_INACTIVE);
    gpio_pin_configure_dt(&orange, GPIO_OUTPUT_INACTIVE);


}

void set_status_led(int status){    

    gpio_pin_set_dt(&green, 0);
    gpio_pin_set_dt(&red, 0);
    gpio_pin_set_dt(&blue, 0);

    switch (status){
        case STATUS_OK:
            gpio_pin_set_dt(&green, 1);
            break;

        case STATUS_ERROR:
            gpio_pin_set_dt(&red, 1);
            break;

        case STATUS_BLOCKED:
            gpio_pin_set_dt(&blue, 1);
            break;

        default:
            break;
    }
}

void halt(char* msg){
    set_status_led(STATUS_ERROR);
    SEGGER_RTT_printf(0, "Error: %s\n", msg);
    k_panic();
}

