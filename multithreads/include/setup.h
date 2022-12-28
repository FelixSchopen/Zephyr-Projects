#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/pwm.h>
#include <zephyr/devicetree.h>
#include "isr.h"

// GPIO specs
extern const struct gpio_dt_spec clk_spec;
extern const struct gpio_dt_spec dt_spec;
extern const struct gpio_dt_spec sw_spec;
extern const struct gpio_dt_spec itr_button_spec;
extern const struct gpio_dt_spec itr_led_spec;

// Devices
extern const struct device* pwm_servo;

// Interrupt callback data
extern struct gpio_callback cb_data;
extern struct gpio_callback cb_data2;

// Basic Variables
extern uint8_t button_pressed;
extern gpio_pin_t clk_last; 
extern gpio_pin_t clk;
extern gpio_pin_t dt;
extern uint16_t pwm_pulse;

// Setup function
void setup(void);

