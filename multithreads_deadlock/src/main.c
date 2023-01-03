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
			if(!deadlock){
				deadlock = 1;
			}
			else{
				mutex1 = 1;
				mutex2 = 1;
				deadlock = 0;
			}
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

void led1_thread(void){
	while(1){
		if(deadlock){
			// Try to lock mutex1
			while(mutex1 != 1) {
				k_msleep(50);
			}
			mutex1 = 0;

			k_usleep(1);

			// Try to lock mutex2
			while(mutex2 != 1){
				k_msleep(50);
			}
			mutex2 = 0;
			
			gpio_pin_toggle_dt(&orange_led_3_spec);
			
			// Unlock mutexes
			mutex1 = 1;
			mutex2 = 1;
		} 
		else{
			gpio_pin_toggle_dt(&orange_led_3_spec);
		}
		k_msleep(50);
	}
}

void led2_thread(void){
	while(1){
		if(deadlock){
			k_msleep(1);
			// Try to lock mutex2
			while(mutex2 != 1) {
				k_msleep(50);
			}
			mutex2 = 0;

			// Try to lock mutex1
			while(mutex1 != 1){
				k_msleep(50);
			}
			mutex1 = 0;

			gpio_pin_toggle_dt(&blue_led_6_spec);
			
			// Unlock mutexes
			mutex2 = 1;
			mutex1 = 1;
		}
		else {
			gpio_pin_toggle_dt(&blue_led_6_spec);
		}
		k_msleep(50);
	}
}

K_THREAD_DEFINE(t0, 1024, interrupt_thread, NULL, NULL, NULL, 7, 0, 0);
K_THREAD_DEFINE(t1, 1024, servo_thread, NULL, NULL, NULL, 7, 0, 0);
K_THREAD_DEFINE(t2, 1024, led1_thread, NULL, NULL, NULL, 6, 0, 0);
K_THREAD_DEFINE(t3, 1024, led2_thread, NULL, NULL, NULL, 6, 0, 0);


void main(void){
	setup();
}