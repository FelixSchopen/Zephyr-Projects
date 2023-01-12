/*
 * Copyright (c) 2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/pwm.h>
#include <zephyr/drivers/can.h>
#include <zephyr/drivers/uart.h>
#include <zephyr/devicetree.h>
#include "SEGGER_RTT.h"

const struct device* uart4_dev = DEVICE_DT_GET(DT_NODELABEL(uart4));
unsigned char received_char;

void main(void){

    while(1){
        if(uart_poll_in(uart4_dev, &received_char) == 0){
                SEGGER_RTT_WriteString(0, "Received character");
        }
        else {
            SEGGER_RTT_WriteString(0, "Did not receive character");
        }
        k_sleep(K_MSEC(1000));
    }

}