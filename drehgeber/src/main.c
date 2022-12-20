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

const struct gpio_dt_spec clk_spec = GPIO_DT_SPEC_GET(DT_NODELABEL(clk), gpios);
const struct gpio_dt_spec dt_spec = GPIO_DT_SPEC_GET(DT_NODELABEL(dt), gpios);
const struct gpio_dt_spec sw_spec = GPIO_DT_SPEC_GET(DT_NODELABEL(sw), gpios);
//const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(DT_NODELABEL(led), gpios);

const struct device* pwm_dev = DEVICE_DT_GET(DT_NODELABEL(pwm));


void main(void){

    gpio_pin_configure_dt(&clk_spec, GPIO_INPUT);
    gpio_pin_configure_dt(&dt_spec, GPIO_INPUT);
    gpio_pin_configure_dt(&sw_spec, GPIO_INPUT);
    
    pwm_set(pwm_dev, 2, PWM_MSEC(400), PWM_MSEC(150), PWM_POLARITY_NORMAL);

    gpio_pin_t clk_last = gpio_pin_get_raw(clk_spec.port, clk_spec.pin); 
    gpio_pin_t clk;
    gpio_pin_t dt;

    int pulse = 250;
    pwm_set(pwm_dev, 2, PWM_MSEC(500), PWM_MSEC(pulse), PWM_POLARITY_NORMAL);

    while(1){
        //clk = gpio_pin_get_raw(clk_spec.port, clk_spec.pin); 
        clk = gpio_pin_get_dt(&clk_spec); 
        if(clk != clk_last){
            if(gpio_pin_get_dt(&dt_spec) != clk){
                if(pulse < 500){
                    pulse += 50;
                }
            }
            else{
                if(pulse > 0){
                    pulse -= 50;
                }
            }
        }
        pwm_set(pwm_dev, 2, PWM_MSEC(500), PWM_MSEC(pulse), PWM_POLARITY_NORMAL);
        clk_last = clk;
    }
}
