/*
 * Copyright (c) 2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/pwm.h>
#include <zephyr/drivers/can.h>
#include <zephyr/drivers/uart.h>
#include <zephyr/devicetree.h>
#include <string.h>
#include "SEGGER_RTT_printf.c"

const struct device* uart_dev = DEVICE_DT_GET(DT_NODELABEL(usart));
const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(DT_NODELABEL(red_led_5), gpios);

int ret;
uint8_t rx_buf[32];
int idx = 0;
struct k_timer my_timer;


char* send_buf = "Hello World!\n";

void uart_write(const struct device* device, char* buf, int buf_size){
    for(int i = 0; i < buf_size; i++){
        uart_poll_out(uart_dev, buf[i]);
    }
}

static void rx_callback(const struct device *dev, void *user_data){
    int i = *((int*)user_data);
    if(uart_irq_rx_ready(dev)){
        k_timer_start(&my_timer, K_MSEC(10), K_NO_WAIT);
        uart_fifo_read(dev, &rx_buf[i], 32);
        *((int*)user_data) = i+1;
    }


}      

void uart_timer_cb(struct k_timer *timer_id){

    if(strcmp(rx_buf, "Hallo") == 0){
        SEGGER_RTT_printf(0, "toggle led\n");
        gpio_pin_toggle_dt(&led);
    }

    if(idx != 0){
        SEGGER_RTT_printf(0, "Successfully read data: %s\n", rx_buf);
    }

    memset(&rx_buf[0], 0, sizeof(rx_buf));
    idx = 0;
}

void main(void){

    gpio_pin_configure_dt(&led, GPIO_OUTPUT_INACTIVE);

    k_timer_init(&my_timer, uart_timer_cb, NULL);


    uart_irq_rx_enable(uart_dev); 
    uart_irq_callback_user_data_set(uart_dev, rx_callback, (void*)&idx);

    while(1){

        uart_write(uart_dev, "Hello World\r\n", sizeof("Hello World\r\n"));        
        k_sleep(K_MSEC(2000));
    }

}