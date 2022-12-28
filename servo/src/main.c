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

const struct device* pwm_dev = DEVICE_DT_GET(DT_NODELABEL(pwm1));

void main(void){

    while(1){
        pwm_set(pwm_dev, 2, PWM_MSEC(20), PWM_MSEC(0.5), PWM_POLARITY_NORMAL);
        k_msleep(2000);
        pwm_set(pwm_dev, 2, PWM_MSEC(20), PWM_MSEC(2.5), PWM_POLARITY_NORMAL);
        k_msleep(2000);
    }
}
