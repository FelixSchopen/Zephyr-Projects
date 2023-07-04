#include "../include/cocktails.h"

struct cocktail* current_cocktail = NULL;
int cocktail_size = 0;

/**
 * JSON descriptor for drinks
 * name: string, name of the drink
 * position: number, position on machine
*/
static const struct json_obj_descr drink_des[] = {
    JSON_OBJ_DESCR_PRIM(struct drink, name, JSON_TOK_STRING),
    JSON_OBJ_DESCR_PRIM(struct drink, position, JSON_TOK_NUMBER),
};

/**
 * JSON descriptor for ingredients
 * drink: struct drink
 * amount: number, amount of the ingredient in % 
*/
static const struct json_obj_descr ingredient_des[] = {
    JSON_OBJ_DESCR_OBJECT(struct ingredient, drink, drink_des),
    JSON_OBJ_DESCR_PRIM(struct ingredient, amount, JSON_TOK_NUMBER),
};

/**
 * JSON descriptor for cocktails
 * name: string
 * ingredients: struct ingredient[], array of ingredients for this cocktail
 * ingredient_count: number, length of ingredient array of this coocktail
*/
static const struct json_obj_descr cocktail_des[] = {
    JSON_OBJ_DESCR_PRIM(struct cocktail, name, JSON_TOK_STRING),
    JSON_OBJ_DESCR_OBJ_ARRAY(struct cocktail, ingredients, 4, ingredient_count, ingredient_des, ARRAY_SIZE(ingredient_des)),
    JSON_OBJ_DESCR_PRIM(struct cocktail, ingredient_count, JSON_TOK_NUMBER),
};

/**
 * Array descriptors to save drinks and cocktails in an array-struct
*/
static struct json_obj_descr drinks_array_des[] = {
    JSON_OBJ_DESCR_OBJ_ARRAY(struct drink_array, drinks, 4, count, drink_des, ARRAY_SIZE(drink_des)), 
};
static const struct json_obj_descr cocktails_array_des[] = {
    JSON_OBJ_DESCR_OBJ_ARRAY(struct cocktail_array, cocktails, 50, count, cocktail_des, ARRAY_SIZE(cocktail_des)), 
};

/**
 * Hold arrays for drinks and cocktails
*/
struct drink_array drinks;
struct cocktail_array cocktails;

int initialize_drinks(void){
    return json_arr_parse(drinks_JSON, sizeof(drinks_JSON), drinks_array_des, &drinks);
}

int initialize_cocktails(void){
    return json_arr_parse(cocktails_JSON, sizeof(cocktails_JSON), cocktails_array_des, &cocktails);
}
