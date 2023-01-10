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
#include <stdio.h>
#include "SEGGER_RTT.h"

CAN_MSGQ_DEFINE(counter_msgq, 2);


const struct device* can1_dev = DEVICE_DT_GET(DT_NODELABEL(can1));

void t_sender(void){
    while(1){
        struct can_frame frame = {
            .id = 0x123, 
            .dlc = 1, 
            .flags = CAN_FRAME_IDE
        };

        frame.data[0] = 10;

        if(can_send(can1_dev, &frame, K_MSEC(100), NULL, "Sender Task") != 0){
            SEGGER_RTT_WriteString(0, "Failed to send can_frame!\n");
        }
        else{
            SEGGER_RTT_WriteString(0, "Successfully sent can_frame!\n");
        }
        
        k_msleep(1000);
    }
}

void can_receiver_callback(const struct device *dev, struct can_frame *frame, void *user_data){
    uint8_t data = frame->data[0];
    SEGGER_RTT_WriteString(0, "Received can_frame\n");
    if(data == 10){
        SEGGER_RTT_WriteString(0, "Data: 10\n");
        printf("Test\n");
    }
    if(data != 10){
        SEGGER_RTT_WriteString(0, "Error\n");
    }
}

K_THREAD_DEFINE(sender, 1024, t_sender, NULL, NULL, NULL, -5, 0, 0);

void main(void){ 
    can_set_mode(can1_dev, CAN_MODE_LOOPBACK);
    can_start(can1_dev);

    const struct can_filter my_filter = {
		.id = 0x123,
        .flags = CAN_FILTER_IDE | CAN_EXT_ID_MASK
	};

    int filter_id = can_add_rx_filter(can1_dev, can_receiver_callback, NULL, &my_filter);

    if(filter_id < 0){
        SEGGER_RTT_WriteString(0, "Error while adding filter!\n");
    }
    else {
        SEGGER_RTT_WriteString(0, "Filter added!\n");
    }
  
}
