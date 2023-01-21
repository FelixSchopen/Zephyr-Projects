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
#include <zephyr/irq.h>
#include <zephyr/drivers/kscan.h>


#include <lvgl.h>	
#include "SEGGER_RTT_printf.c"


const struct device* display_dev = DEVICE_DT_GET(DT_NODELABEL(display));
const struct gpio_dt_spec touch_irq = GPIO_DT_SPEC_GET(DT_NODELABEL(irq), gpios);

static struct gpio_callback cb_data;


// Interrupt Service Routine
void touch_isr(void){
      SEGGER_RTT_printf(0, "Interrupt\n");
}


void main(void){

      gpio_pin_configure_dt(&touch_irq, GPIO_INPUT);

      gpio_pin_interrupt_configure_dt(&touch_irq, GPIO_INT_EDGE_TO_ACTIVE);
      gpio_init_callback(&cb_data, touch_isr, BIT(touch_irq.pin));
      gpio_add_callback(touch_irq.port, &cb_data);

      char buf[32];
      int counter = 0;


      display_blanking_off(display_dev);


      lv_obj_set_style_bg_color(lv_scr_act(), lv_color_hex(0xFFFFFFFF), LV_PART_MAIN);

      lv_obj_t * label = lv_label_create(lv_scr_act());

      lv_obj_set_style_text_color(lv_scr_act(), lv_color_hex(0x00000000), LV_PART_MAIN);
      lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);

      lv_task_handler();


	while (1) {
            sprintf(buf, "%d", counter++);
            lv_label_set_text(label, buf);
            lv_task_handler();
            k_msleep(100);
	}

}