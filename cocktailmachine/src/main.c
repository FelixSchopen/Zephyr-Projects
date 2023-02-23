/*
 * Copyright (c) 2017 Linaro Limited
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "../include/setup.h"

int ret;
int stepper_pos_hor = 0;
int stepper_pos_ver = 0;

void reset_positions(void){
	// Move to starting position
	gpio_pin_set_dt(&dir_hor_spec, LEFT);
	gpio_pin_set_dt(&dir_ver_spec, UP);

	while(1){
		if(gpio_pin_get_dt(&limit_sw_ver0_spec) == 0){
			gpio_pin_toggle_dt(&step_hor_spec);
		}
		if(gpio_pin_get_dt(&limit_sw_hor0_spec) == 0){
			gpio_pin_toggle_dt(&step_hor_spec);
		}

		k_usleep(700);
		
		if(gpio_pin_get_dt(&limit_sw_ver0_spec) == 1 && gpio_pin_get_dt(&limit_sw_hor0_spec) == 1){
			gpio_pin_set_dt(&step_hor_spec, 0);
			gpio_pin_set_dt(&step_ver_spec, 0);
			stepper_pos_hor = 0;
    		stepper_pos_ver = 0;
			return;	
		}
	}
}

void move_hor(int step_count){

	if(stepper_pos_hor < step_count){
		gpio_pin_set_dt(&dir_hor_spec, RIGHT);
		while(stepper_pos_hor < step_count){
			k_usleep(700);
			stepper_pos_hor++;
			SEGGER_RTT_printf(0, "hor pos: %d\n", stepper_pos_hor);
		}
	}
	else{
		gpio_pin_set_dt(&dir_hor_spec, LEFT);
		while(stepper_pos_hor > step_count){
			k_usleep(700);
			stepper_pos_hor--;
			SEGGER_RTT_printf(0, "hor pos: %d\n", stepper_pos_hor);
		}
	}
}

void t_horizontal_motor(void){
	int pos;
	struct q_item* item;
	uint32_t target_step_count;
	while(1){
		if(k_sem_take(&move_to_pos_sem, K_MSEC(500)) == 0) {
			SEGGER_RTT_printf(0, "Mixing cocktail...\n");
			SEGGER_RTT_printf(0, "Cocktail: %s\n", next_cocktail->name);
			SEGGER_RTT_printf(0, "Size: %d\n", cocktail_size);

			while(k_queue_is_empty(&position_q) == 0){
				item = k_queue_get(&position_q, K_NO_WAIT);
				SEGGER_RTT_printf(0, "pos: %d\n", item->data.pos);
				SEGGER_RTT_printf(0, "moving to position...\n");

				pos = item->data.pos;

				switch (pos) {
					case 0:
						target_step_count = POS0;
						break;
					case 1:
						target_step_count = POS1;
						break;
					case 2:
						target_step_count = POS2;
						break;
					case 3:
						target_step_count = POS3;
						break;
					default:
						break;
				}

				move_hor(target_step_count);

				k_sem_give(&fill_glass_sem);
				k_sem_take(&move_to_pos_sem, K_FOREVER);
			}
		}
		k_msleep(100);
	}
}

void t_vertical_motor(void){
	uint16_t amount;
	struct q_item* item;
	while(1){
		if(k_sem_take(&fill_glass_sem, K_MSEC(500)) == 0){
			while(k_queue_is_empty(&amount_q) == 0){
				item = k_queue_get(&amount_q, K_NO_WAIT);
				SEGGER_RTT_printf(0, "amount: %d\n", item->data.amount);
				SEGGER_RTT_printf(0, "filling glass...\n");
				//move_vert(pos);
				k_sem_give(&move_to_pos_sem);
				k_sem_take(&fill_glass_sem, K_FOREVER);
			}
		}
		k_msleep(100);
	}
}

K_THREAD_DEFINE(horizontal_motor, 1024, t_horizontal_motor, NULL, NULL, NULL, -7, 0, -1);
K_THREAD_DEFINE(vertical_motor, 1024, t_vertical_motor, NULL, NULL, NULL, -7, 0, -1);

void main(void){
	uart_setup();
	setup();
	isr_setup();

	SEGGER_RTT_printf(0, "Waiting for server...\n");
	while(!initialized){
		uart_write(uart_dev, "init\n", sizeof("init\n"));
		SEGGER_RTT_printf(0, "initializing...\n");
		k_msleep(4000);
	}

	reset_positions();

	k_msleep(1000);
	

	SEGGER_RTT_printf(0, "Ready\n");

	k_thread_start(horizontal_motor);
	k_thread_start(vertical_motor);

	while(1){
		SEGGER_RTT_printf(0, "Waiting for input...\n");
		k_msleep(2000);
	}
}