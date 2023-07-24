/*
 * Copyright (c) 2017 Linaro Limited
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "../include/setup.h"

int ver_ready = 0;
int hor_ready = 0;

void t_horizontal_motor(void){
	int pos;
	struct q_item *item;
	uint32_t target_step_count = 0;
	while (1) {
		if (k_sem_take(&move_to_pos_sem, K_MSEC(500)) == 0) {

			ver_ready = 0;
			hor_ready = 0;

			while (k_queue_is_empty(&position_q) == 0) {
				item = k_queue_get(&position_q, K_NO_WAIT);
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
					halt("invalid position");
					return;
				}

				move_to_pos(target_step_count);
				k_free(item);

				k_sem_give(&fill_glass_sem);
				k_sem_take(&move_to_pos_sem, K_FOREVER);
			}
			
			hor_set_dir(LEFT);
			while(hor_current_pos > 500){
				hor_move_step();
				hor_current_pos--;
			}
			
			unblock_server();
			set_status_led(STATUS_OK);
		}
		k_msleep(100);
	}
}

void t_vertical_motor(void){
	uint16_t amount;
	struct q_item *item;
	while (1) {
		if (k_sem_take(&fill_glass_sem, K_MSEC(500)) == 0) {
			while (k_queue_is_empty(&amount_q) == 0) {
				item = k_queue_get(&amount_q, K_NO_WAIT);
				amount = cocktail_size * item->data.amount * 0.01;

				fill_glass(amount);

				k_free(item);
				k_sem_give(&move_to_pos_sem);
				k_sem_take(&fill_glass_sem, K_FOREVER);
			}
		}
		k_msleep(100);
	}
}

void t_deadlock(void){
	while (1) {
		k_sem_take(&deadlock_sem, K_FOREVER);
		while(1){
			if(access_virtual_resource_2() == -1){
				k_mutex_unlock(&resource_mutex1);
				k_mutex_unlock(&resource_mutex2);
				release_deadlock = 0;
				break;
			}
			k_msleep(100);
		}
	}
}

void t_inversion(void){
	while (1) {
		k_sem_take(&inversion_sem, K_FOREVER);
		block_mutex_inversion();
		k_msleep(100);
	}
}

void t_init_ver_motor(void){
	while(1){
		if(k_sem_take(&init_ver, K_FOREVER) == 0) {
			ver_set_starting_position();
			while(!ver_is_starting_pos() || !hor_is_starting_pos()) {
				k_msleep(10);
			}
			ver_set_dir(DOWN);
			for (int i = 0; i < 3000; i++) {
				ver_move_step();
			}
			if (!ver_is_starting_pos()) {
				k_msleep(200);
				ver_set_starting_position();
				ver_ready = 1;
			} else {
				halt("ver limit switch not working properly");
			}
		}
		k_msleep(1000);
	}
}

void t_init_hor_motor(void){
	while(1){
		if(k_sem_take(&init_hor, K_FOREVER) == 0) {
			hor_set_starting_position();
			while(!ver_is_starting_pos() || !hor_is_starting_pos()) {
				k_msleep(10);
			}
			k_msleep(100);
			hor_set_dir(RIGHT);
			for (int i = 0; i < 500; i++) {
				hor_move_step();
			}
			if (!hor_is_starting_pos()) {
				hor_ready = 1;
			} else {
				halt("hor limit switch not working properly");
			}
		}
		
		k_msleep(1000);
	}
}

K_THREAD_DEFINE(init_ver_motor, 2048, t_init_ver_motor, NULL, NULL, NULL, 1, 0, 0);
K_THREAD_DEFINE(init_hor_motor, 2048, t_init_hor_motor, NULL, NULL, NULL, 1, 0, 0);

K_THREAD_DEFINE(hor_motor, 2048, t_horizontal_motor, NULL, NULL, NULL, 2, 0, -1);
K_THREAD_DEFINE(ver_motor, 2048, t_vertical_motor, NULL, NULL, NULL, 2, 0, -1);

K_THREAD_DEFINE(deadlock, 2048, t_deadlock, NULL, NULL, NULL, 3, 0, 0);
K_THREAD_DEFINE(priority_inversion, 2048, t_inversion, NULL, NULL, NULL, 3, 0, 0);


void main(void){
	setup();
	uart_setup();
	isr_setup();

	set_status_led(STATUS_BLOCKED);

	while (!initialized) {
		SEGGER_RTT_printf(0, "Waiting for server...\n");
		uart_write("init\n", sizeof("init\n"));
		k_msleep(2000);
	}

	reset_and_check();

	while(!ver_ready || !hor_ready){
		k_msleep(10);
	}
	
	k_thread_start(hor_motor);
	k_thread_start(ver_motor);
	unblock_server();
	set_status_led(STATUS_OK);

	while (1) {
		k_msleep(1000);
	}
}
