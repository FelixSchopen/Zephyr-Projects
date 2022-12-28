/*
 * Copyright (c) 2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/irq.h>


static const struct gpio_dt_spec spec_PA5 = GPIO_DT_SPEC_GET(DT_NODELABEL(itr_button), gpios); 
static const struct gpio_dt_spec spec_PA4 = GPIO_DT_SPEC_GET(DT_NODELABEL(itr_led), gpios);
struct gpio_callback button_cb_data;

int interrupt;


// Interrupt Service Routine
void isr_toggle_pin(void){
    interrupt=1;
}

void main(void){
    gpio_pin_configure_dt(&spec_PA4, GPIO_OUTPUT_ACTIVE);
    gpio_pin_configure_dt(&spec_PA5, GPIO_INPUT);

	gpio_pin_interrupt_configure_dt(&spec_PA5, GPIO_INT_EDGE_TO_ACTIVE);
    gpio_init_callback(&button_cb_data, isr_toggle_pin, BIT(spec_PA5.pin));
	gpio_add_callback(spec_PA5.port, &button_cb_data);

    while(1){
        if(interrupt){
            gpio_pin_toggle_dt(&spec_PA4);
            interrupt=0;
        }
        k_msleep(160);
    }
}