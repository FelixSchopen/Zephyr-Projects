#include "../include/motor.h"

// Variables that hold the position of the motors
// Position is a counter of the steps
int current_pos_hor;
int current_pos_ver;

// Checks if vertical motor is at starting position
int ver_is_starting_pos(void){
	return gpio_pin_get_dt(&limit_sw_ver0_spec);
}

// Checks if horizontal motor is at starting position
int hor_is_starting_pos(void){
	return gpio_pin_get_dt(&limit_sw_hor0_spec);
}

// Set direction of vertical motor
void ver_set_dir(int dir){
    gpio_pin_set_dt(&dir_ver_spec, dir);
}

// Set direction of horizontal motor
void hor_set_dir(int dir){
    gpio_pin_set_dt(&dir_hor_spec, dir);
}

// Moves both motors to their starting position
void reset_positions(void){
	ver_set_dir(UP);
    hor_set_dir(LEFT);
	while(1){
		if(!ver_is_starting_pos()){
			gpio_pin_toggle_dt(&step_ver_spec);
		}
		if(!hor_is_starting_pos()){
			gpio_pin_toggle_dt(&step_hor_spec);
		}
		k_usleep(20);
		
		if(ver_is_starting_pos() && hor_is_starting_pos()){
			gpio_pin_set_dt(&step_hor_spec, 0);
			gpio_pin_set_dt(&step_ver_spec, 0);
			current_pos_hor = 0;
    		current_pos_ver = 0;
			return;	
		}
	}
}

// Moves horizontal motor to a targetes position
void move_to_pos(int target_pos){
	if(current_pos_hor < target_pos){
        hor_set_dir(RIGHT);
		while(current_pos_hor < target_pos){
			gpio_pin_set_dt(&step_hor_spec, 1);
			k_usleep(20);
			gpio_pin_set_dt(&step_hor_spec, 0);
			k_usleep(20);
			current_pos_hor++;
			SEGGER_RTT_printf(0, "hor pos: %d\n", current_pos_hor);
		}
	}
	else{
        hor_set_dir(LEFT);
		while(current_pos_hor > target_pos){
			gpio_pin_set_dt(&step_hor_spec, 1);
			k_usleep(20);
			gpio_pin_set_dt(&step_hor_spec, 0);
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
				gpio_pin_set_dt(&step_ver_spec, 1);
				k_usleep(20);
				gpio_pin_set_dt(&step_ver_spec, 0);
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
            gpio_pin_set_dt(&step_ver_spec, 1);
            k_usleep(20);
            gpio_pin_set_dt(&step_ver_spec, 0);
            k_usleep(20);
		}
	}
}

