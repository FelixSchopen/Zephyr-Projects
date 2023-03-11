/*
 * Copyright (c) 2017 Linaro Limited
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "../include/setup.h"

int limit_switch_error = 0;

void t_horizontal_motor(void)
{
	int pos;
	struct q_item *item;
	uint32_t target_step_count = 0;
	while (1) {
		if (k_sem_take(&move_to_pos_sem, K_MSEC(500)) == 0) {

			k_mutex_lock(&block_mutex, K_FOREVER);
			resource++;
			k_mutex_unlock(&block_mutex);

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
			hor_set_starting_position();
			unblock_server();
			set_status_led(STATUS_OK);
		}
		k_msleep(100);
	}
}

void t_vertical_motor(void)
{
	uint16_t amount;
	struct q_item *item;
	while (1) {
		if (k_sem_take(&fill_glass_sem, K_MSEC(500)) == 0) {
			while (k_queue_is_empty(&amount_q) == 0) {
				item = k_queue_get(&amount_q, K_NO_WAIT);
				amount = item->data.amount;
				amount = cocktail_size * (amount / 100);
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

void t_led(void)
{
	while (1) {
		// gpio_pin_toggle_dt(&orange);
		access_shared_resource2();
		k_msleep(101);
	}
}

void t_access_resource(void)
{
	while (1) {
		if (k_sem_take(&resource_sem, K_FOREVER) == 0) {
			if (k_mutex_lock(&block_mutex, K_NO_WAIT) == 0) {
				SEGGER_RTT_printf(0, "Resource blocked\n");
				resource = 1;
				k_msleep(10000);
				k_mutex_unlock(&block_mutex);
			}
		}
		k_msleep(100);
	}
}

void t_init_ver_motor(void)
{
	while (k_sem_take(&init_ver, K_FOREVER)) {
		ver_set_starting_position();
		if (ver_is_starting_pos() && hor_is_starting_pos()) {
			ver_set_dir(DOWN);
			for (int i = 0; i < 3000; i++) {
				ver_move_step();
			}
			if (!ver_is_starting_pos()) {
				ver_set_starting_position();
			} else {
				halt("ver limit switch not wokring properly");
			}
		}
		k_msleep(1000);
	}
}

void t_init_hor_motor(void)
{
	while (k_sem_take(&init_hor, K_FOREVER)) {
		hor_set_starting_position();
		if (ver_is_starting_pos() && hor_is_starting_pos()) {
			hor_set_dir(RIGHT);
			for (int i = 0; i < 500; i++) {
				hor_move_step();
			}
			if (!hor_is_starting_pos()) {
				hor_set_starting_position();
			} else {
				halt("hor limit switch not wokring properly");
			}
		}
		k_msleep(1000);
	}
}

K_THREAD_DEFINE(hor_motor, 2048, t_horizontal_motor, NULL, NULL, NULL, 4, 0, -1);
K_THREAD_DEFINE(ver_motor, 2048, t_vertical_motor, NULL, NULL, NULL, 4, 0, -1);
K_THREAD_DEFINE(led_thread, 2048, t_led, NULL, NULL, NULL, 5, 0, 0);
K_THREAD_DEFINE(access_resource, 2048, t_access_resource, NULL, NULL, NULL, 8, 0, 0);
K_THREAD_DEFINE(init_ver_motor, 2048, t_init_ver_motor, NULL, NULL, NULL, 1, 0, 0);
K_THREAD_DEFINE(init_hor_motor, 2048, t_init_hor_motor, NULL, NULL, NULL, 1, 0, 0);

void main(void)
{
	setup();
	uart_setup();
	isr_setup();

	set_status_led(STATUS_BLOCKED);

	SEGGER_RTT_printf(0, "%d\n", ver_is_starting_pos());
	SEGGER_RTT_printf(0, "%d\n", hor_is_starting_pos());

	while (!initialized) {
		SEGGER_RTT_printf(0, "Waiting for server...\n");
		uart_write("init\n", sizeof("init\n"));
		k_msleep(2000);
	}

	reset_and_check();

	k_thread_start(hor_motor);
	k_thread_start(ver_motor);
	unblock_server();

	set_status_led(STATUS_OK);

	while (1) {
		k_msleep(1000);
	}
}
