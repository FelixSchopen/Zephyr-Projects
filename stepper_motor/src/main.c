/*
 * Copyright (c) 2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/pwm.h>
#include <zephyr/devicetree.h>
#include <zephyr/sys/printk.h>

const struct gpio_dt_spec dir_spec = GPIO_DT_SPEC_GET(DT_NODELABEL(dir), gpios);
const struct gpio_dt_spec step_spec = GPIO_DT_SPEC_GET(DT_NODELABEL(step), gpios);
//const struct gpio_dt_spec enable_spec = GPIO_DT_SPEC_GET(DT_NODELABEL(enable), gpios);

void main(void){

    gpio_pin_configure_dt(&dir_spec, GPIO_OUTPUT_INACTIVE);
    gpio_pin_configure_dt(&step_spec, GPIO_OUTPUT_INACTIVE);
    //gpio_pin_configure_dt(&enable_spec, GPIO_OUTPUT_ACTIVE);

    gpio_pin_set_dt(&dir_spec, 0);

    for(int i = 0; i<200; i++){
        gpio_pin_set_dt(&step_spec, 1);
        k_usleep(700);
        gpio_pin_set_dt(&step_spec, 0);
        k_usleep(700);
    }

    k_msleep(1000);



    /*while(1){

        gpio_pin_set_dt(&dir_spec, 0);
        for(int i = 0; i<200; i++){
            gpio_pin_set_dt(&step_spec, 1);
            k_usleep(1000);
            gpio_pin_set_dt(&step_spec, 0);
            k_usleep(1000);
        }

        k_msleep(1000);

        gpio_pin_set_dt(&dir_spec, 1);
        for(int i = 0; i<200; i++){
            gpio_pin_set_dt(&step_spec, 1);
            k_usleep(1000);
            gpio_pin_set_dt(&step_spec, 0);
            k_usleep(1000);
        }
        
        k_msleep(1000);
    }*/
}
