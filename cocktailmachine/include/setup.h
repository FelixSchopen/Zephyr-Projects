#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/irq.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/pwm.h>
#include <zephyr/drivers/uart.h>
#include <zephyr/devicetree.h>
#include <string.h>
#include <stdio.h>
#include <zephyr/data/json.h>
#include <zephyr/sys/reboot.h>

#include "SEGGER_RTT.h"

#include "cocktails.h"
#include "uart.h"
#include "isr.h"
#include "motor.h"
#include "realtimeproblems.h"

/*
 * Macros
 */
#define STATUS_OK 0
#define STATUS_ERROR -1
#define STATUS_BLOCKED 1

extern int debug;

/**
 * GPIOs
 */ 
extern const struct gpio_dt_spec hor_dir_spec;
extern const struct gpio_dt_spec hor_step_spec;
extern const struct gpio_dt_spec ver_dir_spec;
extern const struct gpio_dt_spec ver_step_spec;

extern const struct gpio_dt_spec hor_limit_sw0_spec;
extern const struct gpio_dt_spec limit_sw_hor1_spec;
extern const struct gpio_dt_spec ver_limit_sw0_spec;
extern const struct gpio_dt_spec limit_sw_ver1_spec;

extern const struct gpio_dt_spec testpin;

extern const struct gpio_dt_spec green;
extern const struct gpio_dt_spec red ;

extern const struct gpio_dt_spec blue;
extern const struct gpio_dt_spec orange;


/**
 * Devices
 */

//extern struct k_tid_t led; 


// Interrupt callback data

/**
 * Semaphores
 */
extern struct k_sem move_to_pos_sem;
extern struct k_sem fill_glass_sem;
extern struct k_sem drink_sem;
extern struct k_sem cocktail_sem;
extern struct k_sem cmd_sem;


/**
 * Queues
 */
extern struct k_queue position_q;
extern struct k_queue amount_q;

/**
 * TIDs
*/
extern const k_tid_t hor_motor;
extern const k_tid_t ver_motor;
extern const k_tid_t led_thread;


/**
 * Global variables
 */
extern char drinks_JSON[512];
extern char cocktails_JSON[2048];

extern int ready;

extern int current_pos_hor;
extern int current_pos_ver;


/*
 * Functions
 */ 
void setup(void);
void set_status_led(int status);
