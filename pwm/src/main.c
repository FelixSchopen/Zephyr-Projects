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

void main(void){

    const struct device* pwm = device_get_binding("PWM");
    pwm_set(pwm, 2, PWM_SEC(2), PWM_SEC(1), PWM_POLARITY_NORMAL);

    while(1){

    }
}
