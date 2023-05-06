#ifndef PAIRING_PARENTS_H
#define PAIRING_PARENTS_H
#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/**
 * @brief Struct representing the parents selected for crossover.
 * The Parents struct contains the indices of the two parents selected for crossover.
*/
typedef struct parents {
    int p1; /**< Index of the first parent */
    int p2; /**< Index of the second parent */
} Parents;

/**
 * @brief Function pointer type for a pairing function that takes a population of
 * individuals and returns pairs of indices for parents.
 *
 * @param fitnesses Array of fitness scores for each individual in the population.
 * @param pop_size Number of individuals in the population.
 * @param optional_data Optional pointer to additional data needed by the pairing function.
 * @return Parents* Pointer to an array of Parents struct representing the pairs of
 * indices for parents in the population.
*/
typedef Parents* (*PairingFunction)(int *, int, void *);

Parents * consecutive_pairing_parents(int *selected_indices, int selected_size,  void * optional_datas);
Parents * random_pairing_parents(int *selected_indices, int selected_size, void * optional_datas);
Parents * non_sequential_pairing_parents(int *selected_indices, int selected_size, void * optional_datas);

#endif
