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

#define POS0 65200
#define POS1 106500
#define POS2 14600
#define POS3 186000

#define FILL_POS 20000
#define FILL_TIME_MS 1000 

int ver_is_starting_pos(void);

int hor_is_starting_pos(void);

void ver_set_dir(int dir);

void hor_set_dir(int dir);

void set_starting_positions(void);

int reset_and_check(void);

void move_to_pos(int target_pos);

void fill_glass(int ml);

#endif // MOTOR