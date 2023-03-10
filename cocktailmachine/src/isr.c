#include "../include/isr.h"

struct gpio_callback cb_data0;
struct gpio_callback cb_data1;
struct gpio_callback cb_data2;
struct gpio_callback cb_data3;

void isr_hor_limit_sw0(void){
    // stop motor
    SEGGER_RTT_printf(0, "hor0\n");
}

void isr_ver_limit_sw0(void){
    // stop motor 
    // change direction ? 
    SEGGER_RTT_printf(0, "ver0\n");
}

void isr_user_button(){
    SEGGER_RTT_printf(0, "user button\n");
    halt("user aborted");
}

void isr_setup(void){

	gpio_pin_interrupt_configure_dt(&hor_limit_sw0_spec, GPIO_INT_EDGE_TO_ACTIVE);
    gpio_init_callback(&cb_data0, (gpio_callback_handler_t)isr_hor_limit_sw0, BIT(hor_limit_sw0_spec.pin));
	gpio_add_callback(hor_limit_sw0_spec.port, &cb_data0);

    gpio_pin_interrupt_configure_dt(&ver_limit_sw0_spec, GPIO_INT_EDGE_TO_ACTIVE);
    gpio_init_callback(&cb_data1, (gpio_callback_handler_t)isr_ver_limit_sw0, BIT(ver_limit_sw0_spec.pin));
	gpio_add_callback(ver_limit_sw0_spec.port, &cb_data1);

    gpio_pin_interrupt_configure_dt(&user_button_spec, GPIO_INT_EDGE_TO_ACTIVE);
    gpio_init_callback(&cb_data2, (gpio_callback_handler_t)isr_user_button, BIT(user_button_spec.pin));
	gpio_add_callback(user_button_spec.port, &cb_data2);

}