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
#include <zephyr/drivers/adc.h>

static const struct device* adc_dev = DEVICE_DT_GET(DT_NODELABEL(adc));
static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(DT_NODELABEL(red_led_5), gpios);
uint16_t buf;

struct adc_sequence adc_seq = {
    .channels = BIT(8),
    .buffer = &buf,
    .buffer_size = sizeof(buf),
    .resolution = 12,
};

void main(void){
    gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE);
    k_msleep(500);
    while(1){
        if(adc_read(adc_dev, &adc_seq) != 0){
            gpio_pin_set_dt(&led, 0);
        }
        if(*(uint16_t*)adc_seq.buffer > 2000){
            gpio_pin_set_dt(&led, 1);
        }
        else{
            gpio_pin_set_dt(&led, 0);
        }
    }
}
