/*
 * Copyright (c) 2017 Linaro Limited
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "../include/setup.h"

void t_horizontal_motor(void){
	int pos;
	struct q_item* item;
	uint32_t target_step_count = 0;
	while(1){
		if(k_sem_take(&move_to_pos_sem, K_MSEC(500)) == 0) {
			k_mutex_lock(&block_mutex, K_FOREVER);
			resource++;
			k_mutex_unlock(&block_mutex);
			SEGGER_RTT_printf(0, "Mixing cocktail...\n");
			SEGGER_RTT_printf(0, "Cocktail: %s\n", current_cocktail->name);
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
						set_status_led(STATUS_ERROR);
						return;	
				}

				move_to_pos(target_step_count);
				k_free(item);

				k_sem_give(&fill_glass_sem);
				k_sem_take(&move_to_pos_sem, K_FOREVER);
			}
			set_starting_positions();
			unblock_server();
			set_status_led(STATUS_OK);
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
				amount = item->data.amount;
				amount = cocktail_size * (amount/100);
				SEGGER_RTT_printf(0, "amount: %dml\n", item->data.amount);
				SEGGER_RTT_printf(0, "filling glass...\n");

				fill_glass(amount);
				
				k_free(item);
				k_sem_give(&move_to_pos_sem);
				k_sem_take(&fill_glass_sem, K_FOREVER);
			}
		}
		k_msleep(100);
	}
}

void t_led(void){
	while(1){
		gpio_pin_toggle_dt(&orange);
		access_shared_resource2();
		k_msleep(101);
	}
}

void t_access_resource(void){
	while(1){
		if(k_sem_take(&resource_sem, K_FOREVER) == 0){
			if(k_mutex_lock(&block_mutex, K_NO_WAIT) == 0){
				SEGGER_RTT_printf(0, "Resource blocked\n");
				resource = 1;
				k_msleep(10000);
				k_mutex_unlock(&block_mutex);
			}		
		}
		k_msleep(100);
	}
}

K_THREAD_DEFINE(hor_motor, 2048, t_horizontal_motor, NULL, NULL, NULL, 4, 0, -1);
K_THREAD_DEFINE(ver_motor, 2048, t_vertical_motor, NULL, NULL, NULL, 4, 0, -1);
K_THREAD_DEFINE(led_thread, 2048, t_led, NULL, NULL, NULL, 5, 0, 0);
K_THREAD_DEFINE(access_resource, 2048, t_access_resource, NULL, NULL, NULL, 8, 0, 0);


void main(void){
	setup();
	uart_setup();
	isr_setup();

	set_status_led(STATUS_BLOCKED);
	
	while(!initialized){
		SEGGER_RTT_printf(0, "Waiting for server...\n");
		uart_write("init\n", sizeof("init\n"));
		k_msleep(1000);
	}

	if(reset_and_check() != 0){
		set_status_led(STATUS_ERROR);
		return; 
	}

	k_thread_start(hor_motor);
	k_thread_start(ver_motor);
	set_status_led(STATUS_OK);

	unblock_server();
	while(1){
		k_msleep(1000);
	}
}
