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
#include <zephyr/drivers/i2c.h>

static const struct device* i2c_dev = DEVICE_DT_GET(DT_NODELABEL(i2c2));
static const struct gpio_dt_spec red_led = GPIO_DT_SPEC_GET(DT_NODELABEL(red_led_5), gpios);
//static const struct i2c_dt_spec i2c_spec = I2C_DT_SPEC_GET(DT_NODELABEL(i2c2));

uint8_t buf = 255;
uint8_t buf2 = 0;
int ret; 
void main(void){ 

    gpio_pin_configure_dt(&red_led, GPIO_OUTPUT_LOW);
    if(device_is_ready(i2c_dev)){
        gpio_pin_set_dt(&red_led, 1);
    }
    else{
        gpio_pin_set_dt(&red_led, 0);
    }
    k_msleep(200);

    ret = i2c_write(i2c_dev, &buf, 1, 0x50 | 0b111);
    
    if(ret != 0){
        gpio_pin_set_dt(&red_led, 0);
    }
    else{
        gpio_pin_set_dt(&red_led, 1);
    }

    k_msleep(500);


    i2c_read(i2c_dev, &buf2, 1, 0x50);
    if(buf2 == 10){
        gpio_pin_set_dt(&red_led, 1);
    }
    else{
        gpio_pin_set_dt(&red_led, 0);
    }

    k_msleep(2000);

    if(buf2 == buf){
        gpio_pin_set_dt(&red_led, 1);
    }
    else{
        gpio_pin_set_dt(&red_led, 1);
    }
}
