#include "../include/motor.h"

// Variables that hold the position of the motors
// Position is a counter of the steps
int hor_current_pos;
int ver_current_pos;

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

void ver_move_step(void){
	gpio_pin_toggle_dt(&ver_step_spec);
	k_usleep(VER_STEP_DELAY);
	gpio_pin_toggle_dt(&ver_step_spec);
	k_usleep(VER_STEP_DELAY);
	if(deadlock){
		access_shared_resource1();
	}
}

void hor_move_step(void){
	gpio_pin_toggle_dt(&hor_step_spec);
	k_usleep(HOR_STEP_DELAY);
	gpio_pin_toggle_dt(&hor_step_spec);
	k_usleep(HOR_STEP_DELAY);
	if(deadlock){
		access_shared_resource1();
	}
}

// Moves hor motor to starting position
void hor_set_starting_position(void){
    hor_set_dir(LEFT);
	while(!hor_is_starting_pos()){
		hor_move_step();
	}
	gpio_pin_set_dt(&hor_step_spec, 0);
	hor_current_pos = 0;
}

void ver_set_starting_position(void){
	ver_set_dir(UP);
	while(!ver_is_starting_pos()){
		ver_move_step();
	}
	gpio_pin_set_dt(&ver_step_spec, 0);
	ver_current_pos = 0;
}

// Moves both motors to starting positions and checks if limit switches work correctly
void reset_and_check(void){
	k_sem_give(&init_ver);
	k_sem_give(&init_hor);
}

// Moves horizontal motor to a targetes position
void move_to_pos(int target_pos){
	if(hor_current_pos < target_pos){
        hor_set_dir(RIGHT);
		while(hor_current_pos < target_pos){
			hor_move_step();
			hor_current_pos++;
			SEGGER_RTT_printf(0, "hor pos: %d\n", hor_current_pos);
		}
	}
	else{
        hor_set_dir(LEFT);
		while(hor_current_pos > target_pos){
			hor_move_step();
			hor_current_pos--;
			SEGGER_RTT_printf(0, "hor pos: %d\n", hor_current_pos);
		}
	}
}

// Fills glass with the desired amount in ml by 
// repeatedly moving the vertical motor up and down 
void fill_glass(int ml){
	int remaining_ml = ml;
	int next_fill_pos;
	int next_fill_time;

	while(1){
		if(remaining_ml == 0){
			return;
		}

		ver_set_dir(DOWN);
		next_fill_pos = FILL_POS;
		next_fill_time = FILL_TIME_MS;

		if(remaining_ml < 10){
			next_fill_pos -= 1000;
			next_fill_time = 0;
		}
		else if(remaining_ml < 20){
			next_fill_pos -= 600;
			next_fill_time = 0;
		}
		else if (remaining_ml < 30){
			next_fill_pos -= 200;
			next_fill_time = 0;
		}
		else if (remaining_ml < 40){
			next_fill_time = 400;
		}

		for(int i = 0; i < next_fill_pos; i++){
			ver_move_step();
		}
		k_msleep(next_fill_time);

		if(remaining_ml >= 40){
			remaining_ml -= 40;
		}
		else {
			remaining_ml = 0;
		}

		ver_set_starting_position();
	}
}

