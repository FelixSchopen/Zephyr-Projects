#include "../include/isr.h"

struct gpio_callback cb_data2;

// User-Button ISR that aborts the system
void isr_user_button(){
    halt("user aborted");
}

// ISR setup
void isr_setup(void){
    gpio_pin_interrupt_configure_dt(&user_button_spec, GPIO_INT_EDGE_TO_ACTIVE);
    gpio_init_callback(&cb_data2, (gpio_callback_handler_t)isr_user_button, BIT(user_button_spec.pin));
	gpio_add_callback(user_button_spec.port, &cb_data2);

}