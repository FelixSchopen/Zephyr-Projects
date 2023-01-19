/*
 * Copyright (c) 2017 Linaro Limited
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "../include/setup.h"

// Move glass to the next position
void t_motor_glas(void){
	while(1){
	}
}

// Move drinks up/down to fill glass
void t_motor_drinks(void){
	while(1){
	}
}

/* 	Je nach Cocktail muss das Glas in verschiedene Positionen 
	bewegt werden. Dieser Thread kann dazu verwendet werden 
	die Positionen festzulegen. (?)
*/ 
void t_cocktail(void){
	while(1){
	}
}

// Execute commands from user
void t_execute_commands(void){
	while(1){
	}
}

// Receive commands from user
void t_receive_commands(void){
	while(1){
	}
}

// Updates GUI 
void t_gui(void){
	while(1){
	}
}


K_THREAD_DEFINE(motor_glas, 1024, t_motor_glas, NULL, NULL, NULL, -7, 0, 0);
K_THREAD_DEFINE(motor_drinks, 1024, t_motor_drinks, NULL, NULL, NULL, -7, 0, 0);
K_THREAD_DEFINE(execute_commands, 1024, t_execute_commands, NULL, NULL, NULL, -7, 0, 0);
K_THREAD_DEFINE(receive_commands, 1024, t_receive_commands, NULL, NULL, NULL, -7, 0, 0);
K_THREAD_DEFINE(cocktail, 1024, t_cocktail, NULL, NULL, NULL, -7, 0, 0);
K_THREAD_DEFINE(gui, 1024, t_gui, NULL, NULL, NULL, -7, 0, 0);


void main(void){
	setup();
}