#ifndef MUTATION_H
#define MUTATION_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <individual.h>

/**
 * @brief Mutation function type.
 * A mutation function takes an individual and performs a mutation on its genome.
 * The resulting individual is then returned. The function takes an optional
 * parameter to allow passing of additional data needed for the mutation.
 * @param individual The individual to mutate.
 * @param optional_data Optional data needed for the mutation.
 * @return The mutated individual.
*/
typedef Individual (*MutationFunction)(Individual, void *);

Individual random_mutate(Individual c, void * optional_datas);
Individual subsequence_inversion_mutate(Individual c, void * optional_datas);
Individual swap_mutate(Individual c, void * optional_datas);
Individual insertion_mutate(Individual c, void *optional_datas);
Individual deletion_mutate(Individual c, void *optional_datas);

#endif
