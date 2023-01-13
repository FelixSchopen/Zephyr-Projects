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
#include <zephyr/drivers/display.h>
#include <stdio.h>

#include <string.h>

#include "SEGGER_RTT_printf.c"

const struct gpio_dt_spec chip_select = GPIO_DT_SPEC_GET(DT_NODELABEL(cs), gpios);

//const struct device* spi_dev = DEVICE_DT_GET(DT_NODELABEL(spi));
const struct device* display_dev = DEVICE_DT_GET(DT_NODELABEL(display));

void* data = "Hello World";
int ret;

void main(void){ 

   gpio_pin_configure_dt(&chip_select, GPIO_OUTPUT_ACTIVE);
   uint32_t len = strlen(data);

   const struct display_buffer_descriptor des = {
      .width = 100,
      .height = 20,
      .pitch = 5,
      .buf_size = len
   };

	if (display_dev == NULL || !device_is_ready(display_dev)) {
		SEGGER_RTT_WriteString(0, "device not found.  Aborting test.\n");
	}
   else { 
		SEGGER_RTT_WriteString(0, "display found\n");
	}

   while(1){
      struct display_capabilities display_cap;
      display_get_capabilities(display_dev, &display_cap);

      int ret = display_write(display_dev, 50, 50, &des, (void*)data);
      if(ret != 0){
		   SEGGER_RTT_WriteString(0, "error\n");
         SEGGER_RTT_printf(0, "error: %d\n", ret);
      }
      else {
         SEGGER_RTT_printf(0, "success\n");
      }

      SEGGER_RTT_printf(0, "x res: %d\n", display_cap.x_resolution);
      SEGGER_RTT_printf(0, "x res: %d\n", display_cap.x_resolution);


      k_msleep(5000);
   }
}

