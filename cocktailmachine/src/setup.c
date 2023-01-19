#include "../include/setup.h"

const struct gpio_dt_spec clk_spec = GPIO_DT_SPEC_GET(DT_NODELABEL(clk), gpios);
const struct gpio_dt_spec dt_spec = GPIO_DT_SPEC_GET(DT_NODELABEL(dt), gpios);
const struct gpio_dt_spec sw_spec = GPIO_DT_SPEC_GET(DT_NODELABEL(sw), gpios);
const struct gpio_dt_spec itr_button_spec = GPIO_DT_SPEC_GET(DT_NODELABEL(itr_button), gpios);
const struct gpio_dt_spec itr_led_spec = GPIO_DT_SPEC_GET(DT_NODELABEL(itr_led), gpios);

const struct device* pwm_servo = DEVICE_DT_GET(DT_NODELABEL(servo));

// Interrupt callback data
struct gpio_callback cb_data;
struct gpio_callback cb_data2;

// Basic Variables
uint8_t button_pressed = 0;
gpio_pin_t clk_last; 
gpio_pin_t clk;
gpio_pin_t dt;
uint16_t pwm_pulse = 2400;

void setup(void){

    gpio_pin_configure_dt(&clk_spec, GPIO_INPUT);
    gpio_pin_configure_dt(&dt_spec, GPIO_INPUT);
    gpio_pin_configure_dt(&sw_spec, GPIO_INPUT);
    gpio_pin_configure_dt(&itr_led_spec, GPIO_OUTPUT_ACTIVE);
    gpio_pin_configure_dt(&itr_button_spec, GPIO_INPUT);

	gpio_pin_interrupt_configure_dt(&itr_button_spec, GPIO_INT_EDGE_TO_ACTIVE);
    gpio_init_callback(&cb_data, (gpio_callback_handler_t)isr_button, BIT(itr_button_spec.pin));
	gpio_add_callback(itr_button_spec.port, &cb_data);

	gpio_pin_interrupt_configure_dt(&clk_spec, GPIO_INT_EDGE_BOTH);
    gpio_init_callback(&cb_data2, (gpio_callback_handler_t)isr_rotary_encoder, BIT(clk_spec.pin));
	gpio_add_callback(clk_spec.port, &cb_data2);
    
	clk_last = gpio_pin_get_raw(clk_spec.port, clk_spec.pin); 
}