#ifndef COCKTAILS
#define COCKTAILS

#include "setup.h"

// Struct to describe an available drink on the machine
struct drink {
    char* name;     // Name of the drink
    uint16_t position;   // Position of the bottle
};

//  Struct to describe an ingredient for a specific cocktail
//  i.e. Vodka, 20%
struct ingredient {
    struct drink drink; // Drink 
    uint16_t amount;    // Amount of drink in % to have variable drink sizes
};

// Struct to describe a cocktail 
struct cocktail {
    char* name;      // Name of the cocktail
    struct ingredient ingredients[4]; // Array of the ingredients
    uint16_t ingredient_count;  // Length of the ingredient-array
};

/**
 * Structs to hold arrays of drinks and cocktails
*/
struct drink_array { 
    struct drink drinks[4]; 
    size_t count; 
};

struct cocktail_array { 
    struct cocktail cocktails[50]; 
    size_t count; 
};

/**
 * Stucts that hold the drink and cocktail
*/
extern struct drink_array drinks;
extern struct cocktail_array cocktails;

#endif // COKTAILS

