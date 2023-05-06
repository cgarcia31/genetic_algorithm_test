#include <gene.h>

/**
 * @brief Create a random gene (character)
 * This function generates a random gene (character) by using the rand() function.
 * The generated character will be between MINCHAR and MAXCHAR (inclusive).
 * @return A randomly generated gene (character).
*/
Gene create_gene(){
    Gene rand_int = rand() % (MAXCHAR - MINCHAR + 1) + MINCHAR;
    return rand_int;
}
