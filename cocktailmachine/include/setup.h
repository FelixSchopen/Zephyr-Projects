#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/pwm.h>
#include <zephyr/drivers/uart.h>
#include <zephyr/devicetree.h>
#include <string.h>
#include <stdio.h>
#include <zephyr/data/json.h>

#include "cocktails.h"

#include "SEGGER_RTT.h"

// GPIO specs
extern const struct gpio_dt_spec dir_spec;
extern const struct gpio_dt_spec step_spec ;


// Devices


// Interrupt callback data

// Semaphores 
extern struct k_sem move_to_pos_sem;
extern struct k_sem fill_glass_sem;

// Queues
extern struct k_queue position_q;


// Global Variables
extern char drinks_JSON[512];
extern char cocktails_JSON[2048];

extern int ready;


// Setup function
void setup(void);

int initialize_cocktails(void);
int initialize_drinks(void);

