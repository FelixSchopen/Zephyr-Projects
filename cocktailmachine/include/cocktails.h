#include <zephyr/kernel.h>

// Struct to describe an available drink on the machine
struct drink {
    char name[32];      // Name of the drink
    uint32_t position;  // Position of the bottle
};

//  Struct to describe an ingredient for a specific cocktail
//  i.e. Vodka 30ml
struct ingredient {
    struct drink drink; // Drink 
    uint32_t amount;    // Amount of drink in ml
};

// Struct to describe a cocktail 
struct cocktail {
    char name[32];      // Name of the cocktail
    struct ingredient ingredient[]; // Array of the ingredients
};



