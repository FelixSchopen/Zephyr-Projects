/*
 * Copyright (c) 2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>

#define PA4 DT_ALIAS(pina4)

static const struct gpio_dt_spec spec_pa4 = GPIO_DT_SPEC_GET(PA4, gpios);

void main(void){

	gpio_pin_configure_dt(&spec_pa4, GPIO_OUTPUT_ACTIVE);

	while (1) {
		gpio_pin_toggle_dt(&spec_pa4);
		k_msleep(100);
	}
}
