#ifndef CROSSOVER_H
#define CROSSOVER_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <individual.h>

/**
 * @brief Type definition for a crossover function used in genetic algorithms.
 * This typedef defines a function pointer type for crossover functions used in genetic algorithms.
 * A crossover function takes two parent individuals and combines them to produce two child individuals.
 * The exact operation of the crossover function may vary depending on the specific genetic algorithm being used.
 * The void * parameter is a pointer to optional additional data that can be used by the crossover function.
 * @param parent1 The first parent individual
 * @param parent2 The second parent individual
 * @param optional_data A pointer to optional additional data that can be used by the crossover function.
 * @return A tuple containing two child individuals produced by the crossover function.
*/
typedef Individual (*CrossoverFunction)(Individual, Individual, void *);

Individual uniform_crossover(Individual p1, Individual p2, void * optional_datas);
Individual multipoint_crossover(Individual p1, Individual p2, void *optional_datas);
Individual probalistic_crossover(Individual p1, Individual p2, void *optional_datas);

#endif