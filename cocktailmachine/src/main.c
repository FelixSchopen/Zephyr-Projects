/*
 * Copyright (c) 2017 Linaro Limited
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "../include/setup.h"

int ret;

int current_step_count = 0;


void move_hor(int pos){

	int target_step_count;

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

	if(target_step_count > current_step_count){
		// set direction to move right
	}
	else{
		// set direction to move left
	}
	

	for(current_step_count; current_step_count < target_step_count; current_step_count++){
		// move motor
	}
}

void t_move_to_pos(void){
	int pos;
	struct q_item* item;
	while(1){
		if(k_sem_take(&move_to_pos_sem, K_MSEC(500)) == 0) {
			SEGGER_RTT_printf(0, "Mixing cocktail...\n");
			SEGGER_RTT_printf(0, "Cocktail: %s\n", next_cocktail->name);
			SEGGER_RTT_printf(0, "Size: %d\n", cocktail_size);

			/*for(int i = 0; i<4; i++){
				if(next_cocktail->ingredients[i].amount <= 0){
					continue;
				}
				pos = next_cocktail->ingredients[i].drink.position;
				k_sem_give(&fill_glass_sem);
			}*/

			while(k_queue_is_empty(&position_q) == 0){
				item = k_queue_get(&position_q, K_NO_WAIT);
				SEGGER_RTT_printf(0, "pos: %d\n", item->data.pos);
				SEGGER_RTT_printf(0, "moving to position...\n");

				//move_hor(pos);
				k_sem_give(&fill_glass_sem);
				k_sem_take(&move_to_pos_sem, K_FOREVER);
			}
		}
		k_msleep(100);
	}
}

void t_fill_glass(void){
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

K_THREAD_DEFINE(move_to_pos, 1024, t_move_to_pos, NULL, NULL, NULL, -7, 0, -1);
K_THREAD_DEFINE(fill_glass, 1024, t_fill_glass, NULL, NULL, NULL, -7, 0, -1);

void main(void){
	uart_setup();
	SEGGER_RTT_printf(0, "Waiting for server...\n");
	while(!initialized){
		uart_write(uart_dev, "init\n", sizeof("init\n"));
		SEGGER_RTT_printf(0, "initializing...\n");
		k_msleep(4000);
	}
	//setup();
	SEGGER_RTT_printf(0, "Ready\n");

	k_thread_start(move_to_pos);
	k_thread_start(fill_glass);

	while(1){
		SEGGER_RTT_printf(0, "Waiting for input...\n");
		k_msleep(2000);
	}
}