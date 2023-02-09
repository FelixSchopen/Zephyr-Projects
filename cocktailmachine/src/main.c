/*
 * Copyright (c) 2017 Linaro Limited
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "../include/setup.h"
#include "../include/uart_communication.h"
#include "../include/cocktails.h"

int ret;

void main(void){

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