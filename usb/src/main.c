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
#include <zephyr/drivers/usb/usb_dc.h>

#include "SEGGER_RTT.h"

const struct device* usb_dev = DEVICE_DT_GET(DT_NODELABEL(usb));

void main(void){ 
   if(usb_dc_attach() != 0){
      SEGGER_RTT_WriteString(0, "Error\n");
   }
   else {
      SEGGER_RTT_WriteString(0, "Success\n");
   }
   while(1){
      
   }
}
