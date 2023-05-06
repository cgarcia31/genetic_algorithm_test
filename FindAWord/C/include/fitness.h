#ifndef FITNESS
#define FITNESS
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <individual.h>

/**
 * @brief Function pointer type for defining fitness functions.
 * This type defines a function pointer for fitness functions, which takes two strings and an optional data argument
 * and returns a float value representing the fitness of the first string relative to the second string.
 * @param individual A pointer to a string representing the individual whose fitness is being evaluated.
 * @param word A pointer to a string representing the target word or phrase against which the individual's fitness is being evaluated.
 * @param optional_datas An optional pointer to additional data that may be used by the fitness function to calculate the fitness value.
 * @return The fitness value of the individual as a float.
*/
typedef float (*FitnessFunction)(const char *, const char *, void *);


#define swap(x, y) do { \
    int temp_swap = x; \
    x = y; \
    y = temp_swap; \
} while (0)

int len(const char * str);
char * to_set(const char *str);
char * union_set(const char *set1, const char *set2);
char * intersection_set(const char * set1, const char *set2);


float modified_hamming_distance_fitness(const char *individual, const char *word, void * optional_datas);
float levenstein_distance_fitness(const char *individual, const char *word, void * optional_datas);
float smith_waterman(const char *individual, const char *word, void * optional_datas);
float jaccard_similarity_fitness(const char *individual, const char *word, void *optional_datas);
float nlcs_fitness(const char * individual, const char * word, void *optional_datas);
float cosine_similarity_fitness(const char* vec1, const char* vec2, void *optional_datas);
float ngram_overlap_fitness(const char* individual, const char* word, void *optional_datas);
float manhattan_distance_fitness(const char* individual, const char* word, void *optional_datas);
float pearson_correlation_fitness(const char* individual, const char* word, void *optional_datas);

#endif