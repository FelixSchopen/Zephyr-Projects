#include "../include/cocktails.h"


// Struct to describe an available drink on the machine
struct drink {
    char* name;      // Name of the drink
    int position;  // Position of the bottle
};

//  Struct to describe an ingredient for a specific cocktail
//  i.e. Vodka 30ml
struct ingredient {
    struct drink drink; // Drink 
    uint16_t amount;       // Amount of drink in % to have variable drink sizes
};

// Struct to describe a cocktail 
struct cocktail {
    char* name;      // Name of the cocktail
    struct ingredient ingredients[10]; // Array of the ingredients
    uint16_t ingredient_count;
};


static const struct json_obj_descr drink_des[] = {
    JSON_OBJ_DESCR_PRIM(struct drink, name, JSON_TOK_STRING),
    JSON_OBJ_DESCR_PRIM(struct drink, position, JSON_TOK_NUMBER),
};

static const struct json_obj_descr ingredient_des[] = {
    JSON_OBJ_DESCR_OBJECT(struct ingredient, drink, drink_des),
    JSON_OBJ_DESCR_PRIM(struct ingredient, amount, JSON_TOK_NUMBER),
};

static const struct json_obj_descr cocktail_des[] = {
    JSON_OBJ_DESCR_PRIM(struct cocktail, name, JSON_TOK_STRING),
    JSON_OBJ_DESCR_OBJ_ARRAY(struct cocktail, ingredients, 10, ingredient_count, ingredient_des, ARRAY_SIZE(ingredient_des)),
    JSON_OBJ_DESCR_PRIM(struct cocktail, ingredient_count, JSON_TOK_NUMBER),

};

struct drink_struct { 
    struct drink drinks[10]; 
    size_t count; 
};

struct ingredient_struct { 
    struct ingredient ingredients[10]; 
    size_t count; 
};

struct cocktail_struct { 
    struct cocktail cocktails[10]; 
    size_t count; 
};


struct json_obj_descr drinks_array_des[] = {
    JSON_OBJ_DESCR_OBJ_ARRAY(struct drink_struct, drinks, 10, count, drink_des, ARRAY_SIZE(drink_des)), 
};
struct json_obj_descr ingredients_array_des[] = {
    JSON_OBJ_DESCR_OBJ_ARRAY(struct ingredient_struct, ingredients, 10, count, ingredient_des, ARRAY_SIZE(ingredient_des)), 
};
struct json_obj_descr cocktails_array_des[] = {
    JSON_OBJ_DESCR_OBJ_ARRAY(struct cocktail_struct, cocktails, 10, count, cocktail_des, ARRAY_SIZE(cocktail_des)), 
};

struct drink_struct drinks;
struct ingredient_struct ingredients;
struct cocktail_struct cocktails;

int initialize_cocktails(void){
    int ret;
    ret = json_arr_parse(drinks_JSON, sizeof(drinks_JSON), drinks_array_des, &drinks);
    if(ret < 0){
        return ret;
    }
    ret = json_arr_parse(ingredients_JSON, sizeof(ingredients_JSON), ingredients_array_des, &ingredients);
    if(ret < 0){
        return ret;
    }
    ret = json_arr_parse(cocktails_JSON, sizeof(cocktails_JSON), cocktails_array_des, &cocktails);
    return ret;
}
