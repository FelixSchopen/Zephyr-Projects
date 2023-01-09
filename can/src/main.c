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
#include "SEGGER_RTT.h"

const struct device* can1_dev = DEVICE_DT_GET(DT_NODELABEL(can1));

void t_sender(void){
    while(1){
        const struct can_frame frame = {
            .flags = CAN_FRAME_IDE,
            .id = 0x123, 
            .dlc = 1, 
            .data = {10}
        };

        if(can_send(can1_dev, &frame, K_MSEC(100), NULL, NULL) != 0){
            SEGGER_RTT_WriteString(0, "Failed to send can_frame!\n");
        }
        else{
            SEGGER_RTT_WriteString(0, "Successfully sent can_frame!\n");
        }
        
        k_msleep(1000);
    }
}

void can_receiver_callback(const struct device *dev, struct can_frame *frame, void *user_data){

    //uint8_t* data = (*uint8_t)user_data;
    SEGGER_RTT_WriteString(0, "Received can_frame\n");
}

void t_receiver(void){

    void* data;

    const struct can_filter filter = {
        .id = 0x123,
        .mask = 0xFF,
        .res0 = 0xFF,
        .flags = CAN_FILTER_IDE
    };

    while(1){

        if(can_add_rx_filter(can1_dev, (can_rx_callback_t)can_receiver_callback, data, &filter) == filter.id){
            SEGGER_RTT_WriteString(0, "Added can_filter\n");
        }
        else{
            SEGGER_RTT_WriteString(0, "Can not add can_filter\n\n");
        }

        k_msleep(1000);
    }
}

K_THREAD_DEFINE(sender, 1024, t_sender, NULL, NULL, NULL, -5, 0, 0);
K_THREAD_DEFINE(receiver, 1024, t_receiver, NULL, NULL, NULL, -5, 0, 0);


void main(void){ 
    can_set_mode(can1_dev, CAN_MODE_LOOPBACK);
    can_start(can1_dev);
}
