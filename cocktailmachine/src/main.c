/*
 * Copyright (c) 2017 Linaro Limited
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "../include/setup.h"
#include "../include/uart_communication.h"
#include "../include/cocktails.h"

int ret;

void t_move_to_pos(void){
	uint16_t pos;
	while(1){
		if(k_sem_take(&move_to_pos_sem, K_MSEC(500)) != 0){

		}
		else{
			SEGGER_RTT_printf(0, "Mixing cocktail...\n");
			while(k_queue_is_empty(&position_q) == 0){
				pos = *(uint16_t*)k_queue_get(&position_q, K_NO_WAIT);
				SEGGER_RTT_printf(0, "pos: %d\n", pos);
			}
		}
		k_msleep(100);
	}
}

void t_fill_glass(void){
	while(1){
		if(k_sem_take(&fill_glass_sem, K_MSEC(500)) != 0){

		}
		else{
			SEGGER_RTT_printf(0, "Filling glass...\n");
		}
		k_msleep(100);
	}
}

K_THREAD_DEFINE(move_to_pos, 1024, t_move_to_pos, NULL, NULL, NULL, -7, 0, 0);
K_THREAD_DEFINE(fill_glass, 1024, t_fill_glass, NULL, NULL, NULL, -7, 0, 0);


void main(void){
	setup();
	uart_setup();
	SEGGER_RTT_printf(0, "Waiting for server...\n");
	while(!initialized){
		k_msleep(5);
	}	

	SEGGER_RTT_printf(0, "Ready\n");

	while(1){
		k_msleep(100);
	}
}