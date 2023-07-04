#ifndef COCKTAILS
#define COCKTAILS

#include "setup.h"

// Struct to describe an available drink on the machine
// i.e. Vodka, 1
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
    uint16_t ingredient_count;  // Number of fields in the ingredient array that are not null
};

// Structs to hold arrays of drinks and cocktails
// Seems redundant but, because you could simply save the drinks and cocktails directly to an array but is needed by the JSON-Libary
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

extern struct cocktail* current_cocktail;
extern int cocktail_size;

int initialize_cocktails(void);
int initialize_drinks(void);

#endif // COKTAILS

