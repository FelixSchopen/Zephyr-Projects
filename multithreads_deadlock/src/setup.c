#include "../include/setup.h"

const struct gpio_dt_spec clk_spec = GPIO_DT_SPEC_GET(DT_NODELABEL(clk), gpios);
const struct gpio_dt_spec dt_spec = GPIO_DT_SPEC_GET(DT_NODELABEL(dt), gpios);
const struct gpio_dt_spec sw_spec = GPIO_DT_SPEC_GET(DT_NODELABEL(sw), gpios);
const struct gpio_dt_spec itr_button_spec = GPIO_DT_SPEC_GET(DT_NODELABEL(itr_button), gpios);
const struct gpio_dt_spec itr_led_spec = GPIO_DT_SPEC_GET(DT_NODELABEL(itr_led), gpios);
const struct gpio_dt_spec orange_led_3_spec = GPIO_DT_SPEC_GET(DT_NODELABEL(orange_led_3), gpios);
const struct gpio_dt_spec blue_led_6_spec = GPIO_DT_SPEC_GET(DT_NODELABEL(blue_led_6), gpios);

const struct device* pwm_servo = DEVICE_DT_GET(DT_NODELABEL(servo));

struct k_mutex my_mutex1;
struct k_mutex my_mutex2;

// Interrupt callback data
struct gpio_callback cb_data;
struct gpio_callback cb_data2;

// Basic Variables
uint8_t button_pressed = 0;
uint8_t deadlock = 0;
gpio_pin_t clk_last; 
gpio_pin_t clk;
gpio_pin_t dt;
uint16_t pwm_pulse = 600;

uint8_t mutex1 = 1;
uint8_t mutex2 = 1;


void setup(void){

    gpio_pin_configure_dt(&clk_spec, GPIO_INPUT);
    gpio_pin_configure_dt(&dt_spec, GPIO_INPUT);
    gpio_pin_configure_dt(&sw_spec, GPIO_INPUT);
    gpio_pin_configure_dt(&itr_led_spec, GPIO_OUTPUT_ACTIVE);
    gpio_pin_configure_dt(&itr_button_spec, GPIO_INPUT);

    gpio_pin_configure_dt(&orange_led_3_spec, GPIO_OUTPUT_INACTIVE);
    gpio_pin_configure_dt(&blue_led_6_spec, GPIO_OUTPUT_INACTIVE);

	gpio_pin_interrupt_configure_dt(&itr_button_spec, GPIO_INT_EDGE_TO_ACTIVE);
    gpio_init_callback(&cb_data, (gpio_callback_handler_t)isr_toggle_pin, BIT(itr_button_spec.pin));
	gpio_add_callback(itr_button_spec.port, &cb_data);

	gpio_pin_interrupt_configure_dt(&clk_spec, GPIO_INT_EDGE_BOTH);
    gpio_init_callback(&cb_data2, (gpio_callback_handler_t)isr_rotary_encoder, BIT(clk_spec.pin));
	gpio_add_callback(clk_spec.port, &cb_data2);

	clk_last = gpio_pin_get_raw(clk_spec.port, clk_spec.pin); 
}