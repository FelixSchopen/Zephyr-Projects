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
#include <zephyr/devicetree.h>

const struct device* can1_dev = DEVICE_DT_GET(DT_NODELABEL(can1));

void main(void){ 
    int ret;
    can_set_mode(can1_dev, CAN_MODE_LOOPBACK);
    can_start(can1_dev);

    while(1){
        const struct can_frame frame = {
            .flags = CAN_FRAME_IDE,
            .id = 0x123, 
            .dlc = 1, 
            .data = {10}
        };

        if(can_send(can1_dev, &frame, K_MSEC(100), NULL, NULL) != 0){
            
        }
       
        k_msleep(1000);
    }
}
