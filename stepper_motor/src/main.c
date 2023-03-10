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

#include "SEGGER_RTT_printf.c"

#define DOWN 1
#define UP 0
#define RIGHT 1 
#define LEFT 0

const struct gpio_dt_spec dir_spec_ver = GPIO_DT_SPEC_GET(DT_NODELABEL(dir), gpios);
const struct gpio_dt_spec step_spec_ver = GPIO_DT_SPEC_GET(DT_NODELABEL(step), gpios);
const struct gpio_dt_spec enable_ver = GPIO_DT_SPEC_GET(DT_NODELABEL(enable), gpios);

const struct gpio_dt_spec dir_spec_hor = GPIO_DT_SPEC_GET(DT_NODELABEL(dir2), gpios);
const struct gpio_dt_spec step_spec_hor = GPIO_DT_SPEC_GET(DT_NODELABEL(step2), gpios);
const struct gpio_dt_spec enable_hor = GPIO_DT_SPEC_GET(DT_NODELABEL(enable2), gpios);

void hor_move(void){
    gpio_pin_set_dt(&dir_spec_hor, RIGHT);
    while (1){
        for(int i = 0; i<25200; i++){
            gpio_pin_set_dt(&step_spec_hor, 1);
            k_usleep(900);
            gpio_pin_set_dt(&step_spec_hor, 0);
            k_usleep(900);
            SEGGER_RTT_printf(0, "step count: %d\n", i);
        }
        gpio_pin_toggle_dt(&dir_spec_hor);
        k_msleep(2000);
    }
}

void ver_move(void){
    gpio_pin_set_dt(&dir_spec_ver, UP);
    while (1){
        for(int i = 0; i<2000; i++){
            gpio_pin_set_dt(&step_spec_ver, 1);
            k_usleep(20);
            gpio_pin_set_dt(&step_spec_ver, 0);
            k_usleep(20);
            //SEGGER_RTT_printf(0, "step count: %d\n", i);
        }
        gpio_pin_toggle_dt(&dir_spec_ver);
        k_msleep(200);
    }
}

K_THREAD_DEFINE(hor_motor, 2048, hor_move, NULL, NULL, NULL, -3, 0, -1);
K_THREAD_DEFINE(ver_motor, 2048, ver_move, NULL, NULL, NULL, -3, 0, -1);


void main(void){

    gpio_pin_configure_dt(&dir_spec_ver, GPIO_OUTPUT_INACTIVE);
    gpio_pin_configure_dt(&step_spec_ver, GPIO_OUTPUT_INACTIVE);
    gpio_pin_configure_dt(&enable_ver, GPIO_OUTPUT_ACTIVE);

    gpio_pin_configure_dt(&dir_spec_hor, GPIO_OUTPUT_INACTIVE);
    gpio_pin_configure_dt(&step_spec_hor, GPIO_OUTPUT_INACTIVE);
    gpio_pin_configure_dt(&enable_hor, GPIO_OUTPUT_ACTIVE);

    k_msleep(500);

    k_thread_start(hor_motor);
    //k_thread_start(ver_motor);

}
