#include "../include/isr.h"

struct gpio_callback cb_data0;
struct gpio_callback cb_data1;
struct gpio_callback cb_data2;
struct gpio_callback cb_data3;

void isr_limit_sw_hor0(void){
    // stop motor
    SEGGER_RTT_printf(0, "hor0\n");
}

void isr_limit_sw_ver0(void){
    // stop motor 
    // change direction ? 
    SEGGER_RTT_printf(0, "ver0\n");
}

void isr_limit_sw_err(void){
    k_thread_abort(hor_motor);
    k_thread_abort(ver_motor);
    reset_positions();
    k_thread_start(hor_motor);
    k_thread_start(hor_motor);
}

void isr_setup(void){

	gpio_pin_interrupt_configure_dt(&limit_sw_hor0_spec, GPIO_INT_EDGE_TO_ACTIVE);
    gpio_init_callback(&cb_data0, (gpio_callback_handler_t)isr_limit_sw_hor0, BIT(limit_sw_hor0_spec.pin));
	gpio_add_callback(limit_sw_hor0_spec.port, &cb_data0);

	// gpio_pin_interrupt_configure_dt(&limit_sw_hor1_spec, GPIO_INT_EDGE_TO_ACTIVE);
    // gpio_init_callback(&cb_data1, (gpio_callback_handler_t)isr_limit_sw_hor1, BIT(limit_sw_hor1_spec.pin));
	// gpio_add_callback(limit_sw_hor1_spec.port, &cb_data1);

    gpio_pin_interrupt_configure_dt(&limit_sw_ver0_spec, GPIO_INT_EDGE_TO_ACTIVE);
    gpio_init_callback(&cb_data2, (gpio_callback_handler_t)isr_limit_sw_ver0, BIT(limit_sw_ver0_spec.pin));
	gpio_add_callback(limit_sw_ver0_spec.port, &cb_data2);

	// gpio_pin_interrupt_configure_dt(&limit_sw_ver1_spec, GPIO_INT_EDGE_TO_ACTIVE);
    // gpio_init_callback(&cb_data3, (gpio_callback_handler_t)isr_limit_sw_ver1, BIT(limit_sw_ver1_spec.pin));
	// gpio_add_callback(limit_sw_ver1_spec.port, &cb_data3);

}