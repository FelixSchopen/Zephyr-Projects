#include "../include/motor.h"

// Variables that hold the position of the motors
// Position is a counter of the steps
int current_pos_hor;
int current_pos_ver;

// Checks if vertical motor is at starting position
int ver_is_starting_pos(void){
	return gpio_pin_get_dt(&ver_limit_sw0_spec);
}

// Checks if horizontal motor is at starting position
int hor_is_starting_pos(void){
	return gpio_pin_get_dt(&hor_limit_sw0_spec);
}

// Set direction of vertical motor
void ver_set_dir(int dir){
    gpio_pin_set_dt(&ver_dir_spec, dir);
}

// Set direction of horizontal motor
void hor_set_dir(int dir){
    gpio_pin_set_dt(&hor_dir_spec, dir);
}

// Moves both motors to their starting position
void set_starting_positions(void){
	ver_set_dir(UP);
    hor_set_dir(LEFT);
	while(1){
		if(!ver_is_starting_pos()){
			gpio_pin_toggle_dt(&ver_step_spec);
		}
		if(!hor_is_starting_pos()){
			gpio_pin_toggle_dt(&hor_step_spec);
		}
		k_usleep(20);
		
		if(ver_is_starting_pos() && hor_is_starting_pos()){
			gpio_pin_set_dt(&ver_step_spec, 0);
			gpio_pin_set_dt(&hor_step_spec, 0);
			current_pos_hor = 0;
    		current_pos_ver = 0;
			return;	
		}
	}
}

// Moves both motors to starting positions and checks if limit switches work correctly
int reset_and_check(void){
	set_starting_positions();
	ver_set_dir(DOWN);
    hor_set_dir(RIGHT);
	for(int i = 0; i<3000; i++){
		gpio_pin_set_dt(&ver_step_spec, 0);
		gpio_pin_set_dt(&hor_step_spec, 0);
		k_usleep(20);
	}
	if(!ver_is_starting_pos() && !hor_is_starting_pos()){
		set_starting_positions();
		return 0;
	}
	return -1;
}

// Moves horizontal motor to a targetes position
void move_to_pos(int target_pos){
	if(current_pos_hor < target_pos){
        hor_set_dir(RIGHT);
		while(current_pos_hor < target_pos){
			if(deadlock){
				access_shared_resource1();
			}
			gpio_pin_set_dt(&hor_step_spec, 1);
			k_usleep(20);
			gpio_pin_set_dt(&hor_step_spec, 0);
			k_usleep(20);
			current_pos_hor++;
			SEGGER_RTT_printf(0, "hor pos: %d\n", current_pos_hor);
		}
	}
	else{
        hor_set_dir(LEFT);
		while(current_pos_hor > target_pos){
			if(deadlock){
				access_shared_resource1();
			}
			gpio_pin_set_dt(&hor_step_spec, 1);
			k_usleep(20);
			gpio_pin_set_dt(&hor_step_spec, 0);
			k_usleep(20);
			current_pos_hor--;
			SEGGER_RTT_printf(0, "hor pos: %d\n", current_pos_hor);
		}
	}
}

// Fills glass with the desired amount in ml by 
// repeatedly moving the vertical motor up and down 
void fill_glass(int ml){
	int remaining_ml = ml;
	while(1){
        ver_set_dir(DOWN);
		if(remaining_ml == 0){
			return;
		}
		if(remaining_ml < 20){
			// do nothing, prevent glass from over filling
		}
		else {
			for(int i = 0; i < FILL_POS; i++){
				if(deadlock){
					access_shared_resource1();
				}
				gpio_pin_set_dt(&ver_step_spec, 1);
				k_usleep(20);
				gpio_pin_set_dt(&ver_step_spec, 0);
				k_usleep(20);
			}
			if(remaining_ml >= 40){
				k_msleep(FILL_TIME_MS);
				remaining_ml -= 40;
			}
			else {
				remaining_ml = 0;
			}
		}

        ver_set_dir(UP);
		while(!ver_is_starting_pos()){
            gpio_pin_set_dt(&ver_step_spec, 1);
            k_usleep(20);
            gpio_pin_set_dt(&ver_step_spec, 0);
            k_usleep(20);
		}
	}
}

