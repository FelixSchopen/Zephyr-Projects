#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/pwm.h>
#include <zephyr/drivers/uart.h>
#include <zephyr/devicetree.h>
#include <string.h>
#include <stdio.h>
#include <zephyr/data/json.h>
#include "SEGGER_RTT.h"

#include "cocktails.h"
#include "uart.h"
#include "isr.h"


/*
 * Macros
 */
#define DIR_VER_DOWN 0
#define DIR_VER_UP 1
#define DIR_HOR_RIGHT 0
#define DIR_HOR_LEFT 1

#define POS0 1000
#define POS1 2000
#define POS2 3000
#define POS3 4000

extern int debug;


/**
 * GPIOs
 */ 
extern const struct gpio_dt_spec dir_hor_spec;
extern const struct gpio_dt_spec step_hor_spec;
extern const struct gpio_dt_spec dir_ver_spec;
extern const struct gpio_dt_spec step_ver_spec;

extern const struct gpio_dt_spec limit_sw_hor0_spec;
extern const struct gpio_dt_spec limit_sw_hor1_spec;
extern const struct gpio_dt_spec limit_sw_ver0_spec;
extern const struct gpio_dt_spec limit_sw_ver1_spec;


/**
 * Devices
 */

// Interrupt callback data

/**
 * Semaphores
 */
extern struct k_sem move_to_pos_sem;
extern struct k_sem fill_glass_sem;

/**
 * Queues
 */
extern struct k_queue position_q;
extern struct k_queue amount_q;



/**
 * Global variables
 */
extern char drinks_JSON[512];
extern char cocktails_JSON[2048];

extern int ready;


/*
 * Setup functions
 */ 
void setup(void);
