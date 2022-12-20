/*
 * Copyright (c) 2017 Linaro Limited
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/printk.h>
#include <zephyr/sys/__assert.h>
#include <string.h>

/* size of stack area used by each thread */
#define STACKSIZE 1024

/* scheduling priority used by each thread */
#define PRIORITY 7

#define LED0 DT_ALIAS(led0)
#define LED1 DT_ALIAS(led1)
#define LED2 DT_ALIAS(led2)
#define LED3 DT_ALIAS(led3)

static const struct gpio_dt_spec spec_led0 = GPIO_DT_SPEC_GET(LED0, gpios);
static const struct gpio_dt_spec spec_led1 = GPIO_DT_SPEC_GET(LED1, gpios);
static const struct gpio_dt_spec spec_led2 = GPIO_DT_SPEC_GET(LED2, gpios);
static const struct gpio_dt_spec spec_led3 = GPIO_DT_SPEC_GET(LED3, gpios);


void thread0(void){
	gpio_pin_configure_dt(&spec_led0, GPIO_OUTPUT_ACTIVE);
	while(1){
		gpio_pin_toggle_dt(&spec_led0);
		k_msleep(200);
	}
}

void thread1(void){
	gpio_pin_configure_dt(&spec_led1, GPIO_OUTPUT_ACTIVE);
	while(1){
		gpio_pin_toggle_dt(&spec_led1);
		k_msleep(400);
	}
}

void thread2(void){
	gpio_pin_configure_dt(&spec_led2, GPIO_OUTPUT_ACTIVE);
	while(1){
		gpio_pin_toggle_dt(&spec_led2);
		k_msleep(600);
	}
}

void thread3(void){
	gpio_pin_configure_dt(&spec_led3, GPIO_OUTPUT_ACTIVE);
	while(1){
		gpio_pin_toggle_dt(&spec_led3);
		k_msleep(800);
	}
}

const extern k_tid_t t0;
const extern k_tid_t t1;
const extern k_tid_t t2;
const extern k_tid_t t3;

K_THREAD_DEFINE(t0, STACKSIZE, thread0, NULL, NULL, NULL, PRIORITY, 0, 0);
K_THREAD_DEFINE(t1, STACKSIZE, thread1, NULL, NULL, NULL, PRIORITY, 0, 0);
K_THREAD_DEFINE(t2, STACKSIZE, thread2, NULL, NULL, NULL, PRIORITY, 0, 0);
K_THREAD_DEFINE(t3, STACKSIZE, thread3, NULL, NULL, NULL, PRIORITY, 0, 0);

