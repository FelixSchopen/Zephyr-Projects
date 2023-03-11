/*
 * Copyright (c) 2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>


const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(DT_NODELABEL(green_led_2), gpios);

void toggle_led(void){
    gpio_pin_toggle_dt(&led);
}

K_THREAD_DEFINE(t_led, 1024, toggle_led, NULL, NULL, NULL, 1, 0, -1);

void main(void){ 
    gpio_pin_configure_dt(&led, GPIO_OUTPUT_INACTIVE);

    while(1){
        k_thread_start(t_led);
        k_msleep(1000);
    }

}

