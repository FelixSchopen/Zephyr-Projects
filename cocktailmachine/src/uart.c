/*
 * Copyright (c) 2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "../include/uart.h"

const struct device* uart_dev = DEVICE_DT_GET(DT_NODELABEL(usart));

uint8_t initialized = 0;
uint32_t idx = 0;
uint8_t rx_buf[2048];
struct k_timer my_timer;
char cmd_idetifier[32];

char drinks_JSON[512];
char cocktails_JSON[2048];

int update_drinks = 0;


// Write string to UART 
// unused
void uart_write(char* buf, int buf_size){
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
        k_timer_start(&my_timer, K_MSEC(20), K_NO_WAIT);

        uart_fifo_read(dev, &rx_buf[i], 32);
        *((int*)user_data) = i+1;
    }
}      

// Timer callback executes when uart reception completed

void uart_timer_cb(struct k_timer *timer_id){

    // Block server until settings operation finished
    // Check if valid command identifier was received
    if(strcmp(rx_buf, "drinks") == 0 || strcmp(rx_buf, "cocktails") == 0 || strcmp(rx_buf, "mix") == 0){
        strncpy(cmd_idetifier, rx_buf, sizeof(cmd_idetifier));
        memset(&rx_buf[0], 0, sizeof(rx_buf));
        idx = 0;
        return;
    }

    if(strcmp(cmd_idetifier, "drinks") == 0){
        strncpy(drinks_JSON, rx_buf, sizeof(drinks_JSON));
        update_drinks = 1;
    }

    else if(strcmp(cmd_idetifier, "cocktails") == 0){
        strncpy(cocktails_JSON, rx_buf, sizeof(cocktails_JSON));
        if(update_drinks){
            if(initialize_drinks() != 0){
                SEGGER_RTT_printf(0, "Error while initializing drinks\n");
                set_status_led(STATUS_ERROR);
                return;
            }  
            update_drinks = 0;
        }
        if(initialize_cocktails() != 0){            
            SEGGER_RTT_printf(0, "Error while initializing cocktails\n");   
            set_status_led(STATUS_ERROR);
            return;
        }
        if(!initialized){
            initialized = 1;
        }
        else{
            unblock_server();
        }
        set_status_led(STATUS_OK);
        SEGGER_RTT_printf(0, "Settings updated\n");
    }
        

    else if(strcmp(cmd_idetifier, "mix") == 0){
        set_status_led(STATUS_BLOCKED);

        char* ptr;
        int idx = atoi(strtok_r((char*)rx_buf, ",:", &ptr));
        cocktail_size = atoi(strtok_r(NULL, ",:", &ptr));

        current_cocktail = &(cocktails.cocktails[idx]);

        for (int i = 0; i < 4; i++){
            if(current_cocktail->ingredients[i].amount > 0){
                struct q_item* pos_item = k_malloc(sizeof(void*) + sizeof(uint16_t));
                pos_item->data.pos = current_cocktail->ingredients[i].drink.position;

                struct q_item* amount_item = k_malloc(sizeof(void*) + sizeof(uint16_t));
                amount_item->data.amount = current_cocktail->ingredients[i].amount;
                k_queue_append(&position_q, pos_item);
                k_queue_append(&amount_q, amount_item);
            }
        }
        k_sem_give(&move_to_pos_sem);
    }

    else if(strcmp(rx_buf, "deadlock") == 0){
        deadlock = 1; 
    }

    else if(strcmp(rx_buf, "release") == 0){
        deadlock = 0; 
        k_mutex_unlock(&my_mutex1);
        k_mutex_unlock(&my_mutex2);
    }
    else if(strcmp(rx_buf, "inversion") == 0){
        k_sem_give(&resource_sem);
    }
    
    // reset identifier and buffer
    strncpy(cmd_idetifier, "", sizeof(cmd_idetifier));
    memset(&rx_buf[0], 0, sizeof(rx_buf));
    idx = 0; 
}

void uart_setup(void){
    k_timer_init(&my_timer, uart_timer_cb, NULL);
    uart_irq_rx_enable(uart_dev); 
    uart_irq_callback_user_data_set(uart_dev, rx_callback, (void*)&idx);
}

void unblock_server(void){
	uart_write("unblock\n", sizeof("unblock\n"));
}

