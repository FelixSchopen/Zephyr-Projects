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
#include <zephyr/drivers/display.h>
#include <zephyr/drivers/spi.h>
#include <stdio.h>
#include <string.h>
	
#include "SEGGER_RTT_printf.c"


const struct device* spi_dev = DEVICE_DT_GET(DT_NODELABEL(spi));
const struct device* display_dev = DEVICE_DT_GET(DT_NODELABEL(display));
const struct gpio_dt_spec display_chip_select = SPI_CS_GPIOS_DT_SPEC_GET(DT_NODELABEL(display));

char* data = "Hello World";
int ret;
int len;
struct display_capabilities display_cap;

struct display_buffer_descriptor des = {
   .width = 100,
   .height = 20,
   .pitch = 5,
};

void main(void){ 

   len = sizeof(data);
   des.buf_size = len;

   gpio_pin_configure_dt(&display_chip_select, GPIO_OUTPUT_INACTIVE);

	if (display_dev == NULL || !device_is_ready(display_dev)) {
		SEGGER_RTT_WriteString(0, "display not ready.\n");
	}
   	else { 
		SEGGER_RTT_WriteString(0, "display ready\n");
	}

	display_blanking_off(display_dev);
	
   while(1){

		ret = display_write(display_dev, 50, 50, &des, data);
		
		if(ret != 0){
			SEGGER_RTT_printf(0, "display_write error: %d\n", ret);
		}
		else {
			SEGGER_RTT_printf(0, "wrote to display\n");
		}
		k_msleep(5000);
   }
}
