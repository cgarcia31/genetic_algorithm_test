#ifndef POPULATION_H
#define POPULATION_H

#include <individual.h>
#include <fitness.h>
#include <parents.h>
#include <crossover.h>
#include <mutation.h>
#include <selection.h>

#ifndef POP_STRUCT
#define POP_STRUCT
/**
 * @brief Represents a population of individuals.
*/
typedef struct population{
    Individual *individuals;    /**< An array of individuals in the population. */
    int size;                   /**< The number of individuals in the population. */
    int min_individual_size;    /**< The minimum size an individual can have. */
    int max_individual_size;    /**< The maximum size an individual can have. */
    int generation;             /**< The current generation of the population. */
} Population;
#endif

Population create_population(int size, int min_size_individual, int max_size_individual);
Population make_generation(Population p, const char * word, FitnessFunction fitness_function, void *fitness_optional_datas, \
                            SelectionFunction selection_function, void *selection_optional_datas,\
                            PairingFunction pairing_function, void * pairing_optional_datas,\
                            CrossoverFunction crossover_function, void * crossover_optional_datas,\
                            MutationFunction mutation_function, void * mutation_optional_datas);
void free_population(Population p);

#endif
