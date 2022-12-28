/*
 * Copyright (c) 2017 Linaro Limited
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "../include/setup.h"

void interrupt_thread(void){
	while(1){
		if(button_pressed){
			gpio_pin_toggle_dt(&itr_led_spec);
			k_msleep(150);
			button_pressed = 0;
		}
		k_msleep(50);
	}
}

void servo_thread(void){
	while(1){
		pwm_set(pwm_servo, 1, PWM_USEC(20000), PWM_USEC(pwm_pulse), PWM_POLARITY_NORMAL);
		k_msleep(50);
	}
}

K_THREAD_DEFINE(t0, 1024, interrupt_thread, NULL, NULL, NULL, -7, 0, 0);
K_THREAD_DEFINE(t1, 1024, servo_thread, NULL, NULL, NULL, -7, 0, 0);


void main(void){
	setup();
}