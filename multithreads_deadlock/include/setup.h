#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/pwm.h>
#include <zephyr/devicetree.h>
#include <stdio.h>
#include "isr.h"
#include "SEGGER_RTT.h"

#define SERVO_MAX_PULSE 2400
#define SERVO_MIN_PULSE 600

// GPIO specs
extern const struct gpio_dt_spec clk_spec;
extern const struct gpio_dt_spec dt_spec;
extern const struct gpio_dt_spec sw_spec;
extern const struct gpio_dt_spec itr_button_spec;
extern const struct gpio_dt_spec itr_led_spec;
extern const struct gpio_dt_spec orange_led_3_spec;
extern const struct gpio_dt_spec blue_led_6_spec;
extern const struct gpio_dt_spec user_button_spec;

// Devices
extern const struct device* pwm_servo;

// Interrupt callback data
extern struct gpio_callback cb_data;
extern struct gpio_callback cb_data2;
extern struct gpio_callback cb_data3;


// Basic Variables
extern uint8_t button_pressed;
extern uint8_t rotary_encoder_rdy;
extern uint8_t deadlock;
extern gpio_pin_t clk_last; 
extern gpio_pin_t clk;
extern gpio_pin_t dt;
extern uint16_t pwm_pulse;

// Mutex
extern uint8_t mutex1;
extern uint8_t mutex2;

extern struct k_sem my_sem1;


// Setup function
void setup(void);

