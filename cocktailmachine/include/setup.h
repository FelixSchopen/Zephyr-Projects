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



// GPIO specs


// Devices


// Interrupt callback data


// Global Variables
extern char drinks_JSON[512];
extern char ingredients_JSON[512];
extern char cocktails_JSON[512];

extern int ready;


// Setup function
void setup(void);

int initialize_cocktails(void);
int initialize_drinks(void);




