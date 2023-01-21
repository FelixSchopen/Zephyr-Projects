/*
 * Copyright (c) 2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "../include/uart_communication.h"

const struct device* uart_dev = DEVICE_DT_GET(DT_NODELABEL(usart));

uint8_t initialized = 0;
uint32_t idx = 0;
uint8_t rx_buf[512];
struct k_timer my_timer;
char initialization_identifier[32];

char drinks_JSON[512];
char ingredients_JSON[512];
char cocktails_JSON[512];

// Write string to UART 
// nused
void uart_write(const struct device* device, char* buf, int buf_size){
    for(int i = 0; i < buf_size; i++){
        uart_poll_out(uart_dev, buf[i]);
    }
}

// Callback function to handle received data
void rx_callback(const struct device *dev, void *user_data){
    int i = *((int*)user_data);
    if(uart_irq_rx_ready(dev)){

        /*  
            Restart timer every time a new character arrives.
            if no new char arrives within 50ms the timer 
            callback executes and resets the rx_buffer.
            This is used beacuse Zephyr UART-API only reads 
            one character at a time.
        */
        k_timer_stop(&my_timer);
        k_timer_start(&my_timer, K_MSEC(50), K_NO_WAIT);

        uart_fifo_read(dev, &rx_buf[i], 32);
        *((int*)user_data) = i+1;
    }
}      

// Timer callback to signal a completeted UART receipt
void uart_timer_cb(struct k_timer *timer_id){

    if(initialized){
        SEGGER_RTT_printf(0, "Initialization already completed\n");
        return;
    }
    
    if(strcmp(rx_buf, "drinks") == 0 || strcmp(rx_buf, "ingredients") == 0 || strcmp(rx_buf, "cocktails") == 0){
        strncpy(initialization_identifier, rx_buf, sizeof(initialization_identifier));
        memset(&rx_buf[0], 0, sizeof(rx_buf));
        idx = 0;
        return;
    }

    if(strcmp(initialization_identifier, "drinks") == 0){
        strncpy(drinks_JSON, rx_buf, sizeof(drinks_JSON));
    }
    else if(strcmp(initialization_identifier, "ingredients") == 0){
        strncpy(ingredients_JSON, rx_buf, sizeof(ingredients_JSON));
    }
    else if(strcmp(initialization_identifier, "cocktails") == 0){
        strncpy(cocktails_JSON, rx_buf, sizeof(cocktails_JSON));
        initialized = 1;
    }

    strncpy(initialization_identifier, "", sizeof(initialization_identifier));
    memset(&rx_buf[0], 0, sizeof(rx_buf));
    idx = 0; 
}


void uart_setup(void){
    k_timer_init(&my_timer, uart_timer_cb, NULL);
    uart_irq_rx_enable(uart_dev); 
    uart_irq_callback_user_data_set(uart_dev, rx_callback, (void*)&idx);
}
