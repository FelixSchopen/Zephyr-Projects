#include "setup.h"

extern uint8_t initialized;

void uart_write(const struct device* device, char* buf, int buf_size);
void rx_callback(const struct device *dev, void *user_data);
void uart_timer_cb(struct k_timer *timer_id);
void uart_setup(void);

