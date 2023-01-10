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
#include <zephyr/net/mqtt.h>

#include "SEGGER_RTT.h"

/* Buffers for MQTT client. */
static u8_t rx_buffer[256];
static u8_t tx_buffer[256];

/* MQTT client context */
struct mqtt_client client_ctx;
struct sockaddr_storage broker;

void mqtt_evt_handler(struct mqtt_client *client, const struct mqtt_evt *evt){
   switch (evt->type) {
      /* Handle events here. */
   }
}   

void main(void){ 
    
}
