/*
 * Copyright (c) 2017 Linaro Limited
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "../include/setup.h"

void t_button(void){
	while(1){
		if(button_pressed){
			SEGGER_RTT_WriteString(0, "Button pressed!\n");
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
		k_msleep(10);
	}
}

void t_servo(void){
	while(1){
		pwm_set(pwm_servo, 1, PWM_USEC(20000), PWM_USEC(pwm_pulse), PWM_POLARITY_NORMAL);
		k_msleep(50);
	}
}


void t_led1(void){
	while(1){
		if(deadlock){
			// Try to lock mutex1
			while(mutex1 != 1) {
				k_usleep(0.1);
			}
			mutex1 = 0;

			k_usleep(1);

			// Try to lock mutex2
			while(mutex2 != 1){
				k_usleep(0.1);
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

void t_led2(void){
	while(1){
		if(deadlock){
			k_msleep(1);
			// Try to lock mutex2
			while(mutex2 != 1) {
				k_usleep(0.1);
			}
			mutex2 = 0;

			// Try to lock mutex1
			while(mutex1 != 1){
				k_usleep(0.1);
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

K_THREAD_DEFINE(button, 1024, t_button, NULL, NULL, NULL, -7, 0, 0);
K_THREAD_DEFINE(servo, 1024, t_servo, NULL, NULL, NULL, -3, 0, 0);
K_THREAD_DEFINE(led1, 1024, t_led1, NULL, NULL, NULL, -5, 0, 0);
K_THREAD_DEFINE(led2, 1024, t_led2, NULL, NULL, NULL, -5, 0, 0);


void main(void){
	setup();
}