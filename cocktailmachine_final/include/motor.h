#ifndef MOTOR
#define MOTOR

#include "setup.h"

/*
 * Macros
 */
#define DOWN 1
#define UP 0
#define RIGHT 1 
#define LEFT 0

#define POS0 7300
#define POS1 12500
#define POS2 17600
#define POS3 22600

#define FILL_POS 20000
#define FILL_TIME_MS 1500 

#define VER_STEP_DELAY 20
#define HOR_STEP_DELAY 700

int ver_is_starting_pos(void);

int hor_is_starting_pos(void);

void ver_set_dir(int dir);

void hor_set_dir(int dir);

void ver_move_step(void);

void hor_move_step(void);

void ver_set_starting_position(void);

void hor_set_starting_position(void);

void reset_and_check(void);

void move_to_pos(int target_pos);

void fill_glass(int ml);

#endif // MOTOR