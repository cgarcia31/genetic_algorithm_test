#include <individual.h>

/**
 * @brief Create a new individual with random genome of length between min_size_individual and max_size_individual.
 * @param min_size_individual The minimum size of the individual's genome.
 * @param max_size_individual The maximum size of the individual's genome.
 * @return Individual The created individual with random genome and metadata.
*/
Individual create_individual(int min_size_individual, int max_size_individual){
    Individual c;
    int rand_int = rand() % (max_size_individual - min_size_individual +1) + min_size_individual;
    c.genome = malloc(sizeof(Gene) * (rand_int+1));
    int i = 0;
    for (; i < rand_int; i++)
        c.genome[i] = create_gene();
    c.genome[i] = '\0';
    c.size = rand_int+1;
    c.min_size = min_size_individual;
    c.max_size = max_size_individual;
    return c;
}

/**
 * @brief Frees the memory allocated for an Individual structure.
 * This function takes an Individual structure as input and frees the memory allocated for its genome.
 * @param individual The Individual structure to free.
 * @return None.
*/
void free_individual(Individual individual){
    if (individual.genome != NULL){
        free(individual.genome);
    }
    individual.size = 0;
}