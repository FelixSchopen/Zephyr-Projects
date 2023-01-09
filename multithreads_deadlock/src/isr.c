#include "../include/isr.h"
#include "../include/setup.h"

// Button
void isr_button(void){
    button_pressed = 1;
}

// Rotary Encoder
void isr_rotary_encoder(void){
	clk = gpio_pin_get_raw(clk_spec.port, clk_spec.pin);
	dt = gpio_pin_get_raw(dt_spec.port, dt_spec.pin);
	if(clk != clk_last){
		if(dt != clk){			
			if(pwm_pulse < 2400){
				pwm_pulse+=120;
			}
		}
		else{
			if(pwm_pulse > 600){
				pwm_pulse-=120;
			}
		}
	}
	clk_last = clk;
}