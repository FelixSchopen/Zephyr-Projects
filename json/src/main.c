/*
 * Copyright (c) 2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/data/json.h>

#include "SEGGER_RTT_printf.c"

// Struct to describe an available drink on the machine
struct drink {
    char* name;      // Name of the drink
    int position;  // Position of the bottle
};

static const struct json_obj_descr drink_des[] = {
    JSON_OBJ_DESCR_PRIM(struct drink, name, JSON_TOK_STRING),
    JSON_OBJ_DESCR_PRIM(struct drink, position, JSON_TOK_NUMBER),
};

struct a { 
    struct drink drinks[10]; 
    size_t count; 
};
struct json_obj_descr array[] = {
    JSON_OBJ_DESCR_OBJ_ARRAY(struct a, drinks, 10, count, drink_des, ARRAY_SIZE(drink_des)), 
};
struct drink drink;
struct a drinks_f;
void main(void){ 
    while(1){
            char str[] = "{\"name\":\"Vodka\",\"position\":1}";
            char str2[] = "[{\"name\":\"Vodka\",\"position\":1},{\"name\":\"Gin\",\"position\":2}]";

            int ret = json_obj_parse(str, sizeof(str), drink_des, ARRAY_SIZE(drink_des), &drink);
            SEGGER_RTT_printf(0, "Return: %d\n", ret);
            SEGGER_RTT_printf(0, "Drink: %s, %d\n", drink.name, drink.position);

            ret = json_arr_parse(str2, sizeof(str2), array, &drinks_f);
            SEGGER_RTT_printf(0, "Return: %d\n", ret);
            SEGGER_RTT_printf(0, "Drink: %s, %d\n", drinks_f.drinks[0].name, drinks_f.drinks[0].position);
            k_msleep(1000);
    }
}

