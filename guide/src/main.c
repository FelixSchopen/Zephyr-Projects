/*
 * Copyright (c) 2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>


#define ORANGE_LED_NODE DT_NODELABEL(orange_led_3)

const struct gpio_dt_spec orange_led_spec = GPIO_DT_SPEC_GET(ORANGE_LED_NODE, gpios);

void main(void){ 
    gpio_pin_configure_dt(&orange_led_spec, GPIO_OUTPUT_LOW);
    while(1){
        gpio_pin_toggle_dt(&orange_led_spec);
        k_msleep(500);
    }
}

