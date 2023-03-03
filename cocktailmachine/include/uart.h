#ifndef UART
#define UART

#include "setup.h"

extern const struct device* uart_dev;
extern uint8_t initialized;

extern int deadlock;


void uart_write(const struct device* device, char* buf, int buf_size);
void rx_callback(const struct device *dev, void *user_data);
void uart_timer_cb(struct k_timer *timer_id);
void uart_setup(void);


struct q_item {
    void *fifo_reserved;   /* 1st word reserved for use by FIFO */
    union {
        uint16_t pos;
        uint16_t amount;
    } data;
};

#endif // UART

