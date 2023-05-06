
#ifndef SELECTION_H
#define SELECTION_H
#include <string.h>
#include <stdlib.h>
#include <individual.h>

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

// Define a small value to prevent division by zero
#define EPSILON 0.000001f

/**
 * @brief A function pointer type definition for a selection function.
 * This function takes in a population, an array of fitness scores, a selection pressure value, and optional data.
 * It returns an array of indices of the selected individuals from the population for mating.
 * @param population The population to select from.
 * @param fitness_scores An array of fitness scores for the individuals in the population.
 * @param selection_rate A value indicating how much pressure to apply for selecting fitter individuals.
 * @param optional_data Optional data that may be needed by the selection function.
 * @return An array of indices of selected individuals from the population for mating.
*/
typedef int* (*SelectionFunction)(Population, float *, float, void *);

/**
 * @brief Structure representing an individual's index and its corresponding score
*/
typedef struct {
    int idx;        /**< Index of the individual in the population */
    float score;    /**< Score of the individual */
} IndividualScore;

int score_cmp(const void* a, const void* b);


int* truncation_selection(Population p, float * fitness_scores, float selection_rate, void *optional_datas);
int *rank_based_selection(Population p, float * fitness_scores, float selection_rate, void *optional_datas);
int* roulette_wheel_selection(Population p, float * fitness_scores, float selection_rate, void *optional_datas);
int* tournament_selection(Population p, float * fitness_scores, float selection_rate, void *optional_datas);

#endif