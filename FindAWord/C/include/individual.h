#ifndef INDIVIDUAL_H
#define INDIVIDUAL_H

#include <gene.h>

/**
 * @brief Struct representing an individual in a genetic algorithm.
*/
typedef struct individual{
    Gene * genome;  /**< Array of genes representing the individual's genome */
    int size;       /**< Current size of the genome array */
    int min_size;   /**< Minimum size the genome array can be */
    int max_size;   /**< Maximum size the genome array can be */
} Individual;

Individual create_individual(int min_size_individual, int max_size_individual);
void free_individual(Individual individual);

#endif