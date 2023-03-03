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

				move_to_pos(target_step_count);

				k_free(item);
				k_sem_give(&fill_glass_sem);
				k_sem_take(&move_to_pos_sem, K_FOREVER);
			}
			reset_positions();
			uart_write(uart_dev, "unblock\n", sizeof("unblock\n"));
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

void t_uart_commands(void){
	while(1){
		if(k_sem_take(&drink_sem, K_MSEC(10)) == 0){
			if(initialize_drinks() != 0){
				SEGGER_RTT_printf(0, "Error while initializing drinks\n");
				set_status_led(STATUS_ERROR);
			}
			else{
				SEGGER_RTT_printf(0, "Drinks updated\n");
			}
		}		
		if(k_sem_take(&cocktail_sem, K_MSEC(10)) == 0){
			if(initialize_cocktails() != 0){            
				SEGGER_RTT_printf(0, "Error while initializing cocktails\n");   
				set_status_led(STATUS_ERROR);
			}
			else{
				if(!initialized){
					initialized = 1;
				}
				SEGGER_RTT_printf(0, "Cocktails updated\n");
				uart_write(uart_dev, "unblock\n", sizeof("unblock\n"));
				set_status_led(STATUS_OK);
			}  
		}
		k_msleep(100);
	}
}

K_THREAD_DEFINE(horizontal_motor, 2048, t_horizontal_motor, NULL, NULL, NULL, -7, 0, -1);
K_THREAD_DEFINE(vertical_motor, 2048, t_vertical_motor, NULL, NULL, NULL, -7, 0, -1);
K_THREAD_DEFINE(uart_commands, 2048, t_uart_commands, NULL, NULL, NULL, -7, 0, -1);


void main(void){
	setup();
	uart_setup();
	isr_setup();

	set_status_led(STATUS_BLOCKED);
	
	while(!initialized){
		gpio_pin_toggle_dt(&blue);
		SEGGER_RTT_printf(0, "Waiting for server...\n");
		uart_write(uart_dev, "init\n", sizeof("init\n"));
		k_msleep(1000);
	}

	reset_positions();	
	uart_write(uart_dev, "initialized\n", sizeof("initialized\n"));

	k_thread_start(horizontal_motor);
	k_thread_start(vertical_motor);

	set_status_led(STATUS_OK);

	while(1){
		SEGGER_RTT_printf(0, "Waiting for input...\n");
		k_msleep(3000);
	}
}

