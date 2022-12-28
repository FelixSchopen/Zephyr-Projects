/*
 * Copyright (c) 2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/irq.h>

#define PA5_DEV_IRQ 24
#define PA5_DEV_PRIO 2

#define STACKSIZE 1024
#define PRIORITY 7

#define PA4 DT_ALIAS(pina5)
#define PA5 DT_ALIAS(pina4)

#define LED0 DT_ALIAS(led0)
#define LED1 DT_ALIAS(led1)
#define LED2 DT_ALIAS(led2)
#define LED3 DT_ALIAS(led3)

static const struct gpio_dt_spec spec_PA4 = GPIO_DT_SPEC_GET(PA4, gpios); 
static const struct gpio_dt_spec spec_PA5 = GPIO_DT_SPEC_GET(PA5, gpios);
static struct gpio_callback button_cb_data;

static const struct gpio_dt_spec spec_led0 = GPIO_DT_SPEC_GET(LED0, gpios);
static const struct gpio_dt_spec spec_led1 = GPIO_DT_SPEC_GET(LED1, gpios);
static const struct gpio_dt_spec spec_led2 = GPIO_DT_SPEC_GET(LED2, gpios);
static const struct gpio_dt_spec spec_led3 = GPIO_DT_SPEC_GET(LED3, gpios);

int interrupt;

const extern k_tid_t t0;
const extern k_tid_t t1;
const extern k_tid_t t2;
const extern k_tid_t t3;
const extern k_tid_t t4;

void thread0(void){
	while(1){
		gpio_pin_toggle_dt(&spec_led0);
		k_msleep(200);
	}
}

void thread1(void){
	while(1){
		gpio_pin_toggle_dt(&spec_led1);
		k_msleep(400);
	}
}

void thread2(void){
	while(1){
		gpio_pin_toggle_dt(&spec_led2);
		k_msleep(600);
	}
}

void thread3(void){
	while(1){
		gpio_pin_toggle_dt(&spec_led3);
		k_msleep(800);
	}
}

void thread4(void){
	 while(1){
        if(interrupt){
            gpio_pin_toggle_dt(&spec_PA4);
            interrupt=0;
			k_msleep(160);
        }
    }
}

K_THREAD_DEFINE(t0, STACKSIZE, thread0, NULL, NULL, NULL, PRIORITY, 0, 0);
K_THREAD_DEFINE(t1, STACKSIZE, thread1, NULL, NULL, NULL, PRIORITY, 0, 0);
K_THREAD_DEFINE(t2, STACKSIZE, thread2, NULL, NULL, NULL, PRIORITY, 0, 0);
K_THREAD_DEFINE(t3, STACKSIZE, thread3, NULL, NULL, NULL, PRIORITY, 0, 0);
K_THREAD_DEFINE(t4, STACKSIZE, thread4, NULL, NULL, NULL, PRIORITY+1, 0, 0);

// Interrupt Service Routine
void isr_toggle_pin(void){
    interrupt=1;
}

void main(void){
    gpio_pin_configure_dt(&spec_PA4, GPIO_OUTPUT_ACTIVE);
    gpio_pin_configure_dt(&spec_PA5, GPIO_INPUT);

    gpio_pin_configure_dt(&spec_led0, GPIO_OUTPUT_ACTIVE);
	gpio_pin_configure_dt(&spec_led1, GPIO_OUTPUT_ACTIVE);
	gpio_pin_configure_dt(&spec_led2, GPIO_OUTPUT_ACTIVE);
	gpio_pin_configure_dt(&spec_led3, GPIO_OUTPUT_ACTIVE);

	gpio_pin_interrupt_configure_dt(&spec_PA5, GPIO_INT_EDGE_TO_ACTIVE);
    gpio_init_callback(&button_cb_data, isr_toggle_pin, BIT(spec_PA5.pin));
	gpio_add_callback(spec_PA5.port, &button_cb_data);
}
