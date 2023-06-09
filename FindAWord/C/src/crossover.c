#include <crossover.h>

/**
 * @brief Performs a uniform crossover on two parent individuals.
 * This function creates a new individual as the child of the two parent individuals using uniform crossover.
 * A length for the child individual is randomly chosen from either parent.
 * Each character in the child individual is selected from the corresponding parent with a 50% chance for each character.
 * If the child length is greater than the minimum length of the two parents, the remaining characters are either
 * selected from one parent or generated randomly with equal probability.
 * 
 * @param p1 The first parent individual.
 * @param p2 The second parent individual.
 * @param optional_datas Optional parameters for the function.
 * @return The child individual generated by the uniform crossover.
*/
Individual uniform_crossover(Individual p1, Individual p2, void * optional_datas){
    int p1_len, p2_len;
    p1_len = p1.size;
    p2_len = p2.size;

    int min_len = p1_len < p2_len ? p1_len : p2_len;

    // Choose length of child randomly from either parent
    int child_len = rand() % 2 ? p1_len : p2_len;

    // Perform crossover by selecting characters from parents with a 50% chance for each character
    Individual child;
    child.genome = malloc(sizeof(Gene) * (child_len + 1));
    int i;
    for (i = 0; i < min_len; i++) {
        if (rand() % 2 == 0) {
            child.genome[i] = p1.genome[i];
        } else {
            child.genome[i] = p2.genome[i];
        }
    }

    // If child length is greater than min_xy, fill the remaining characters with characters from a parent or randomly
    for (i = min_len; i < child_len; i++) {
        if (rand() % 2 == 0) {
            if (i < p1_len) {
                child.genome[i] = p1.genome[i];
            } else {
                child.genome[i] = create_gene();
            }
        } else {
            if (i < p2_len) {
                child.genome[i] = p2.genome[i];
            } else {
                child.genome[i] = create_gene();
            }
        }
    }

    child.genome[child_len] = '\0';
    child.min_size = p1.min_size;
    child.max_size = p1.max_size;
    child.size = child_len;
    return child;
}

/**
 * @brief Performs a multipoint crossover between two individuals.
 * @param p1 The first parent individual.
 * @param p2 The second parent individual.
 * @param optional_datas Optional parameter to specify the number of crossover points. Default is 2.
 * @return A new individual resulting from the crossover.
*/
Individual multipoint_crossover(Individual p1, Individual p2, void *optional_datas) {
    int nb_points = (optional_datas == NULL) ? 2 : *(int *) optional_datas;
    int p1_len = p1.size, p2_len = p2.size;
    int min_len = (p1_len < p2_len) ? p1_len : p2_len;
    int child_len = ((rand() % 2) == 0) ? p1_len : p2_len;
    int slice_size = min_len / nb_points, min_slice = 0, max_slice = slice_size;

    // Allocate memory for child genome
    Individual child;
    child.genome = malloc((child_len + 1) * sizeof(Gene));
    child.min_size = p1.min_size;
    child.max_size = p1.max_size;
    child.size = child_len;

    // Copy slices from parent genomes
    Gene *parent_genome;
    int i;
    for (i = 0; i < nb_points; i++, min_slice += slice_size, max_slice += slice_size) {
        parent_genome = ((rand() % 2) == 0) ? p1.genome : p2.genome;
        memcpy(child.genome + min_slice, parent_genome + min_slice, (max_slice - min_slice) * sizeof(Gene));
    }

    // Copy remaining genes from parent genome
    parent_genome = (p1_len > p2_len) ? p1.genome : p2.genome;
    int remaining_len = child_len - min_slice;
    memcpy(child.genome + min_slice, parent_genome + min_slice, remaining_len * sizeof(Gene));

    // Add null terminator to child genome
    child.genome[child_len] = '\0';
    return child;
}

/**
 * @brief Performs a probabilistic crossover between two individuals.
 * This crossover method selects genes from each parent to be included in the child with a probability determined
 * by a randomly generated probability array. The size of the child is randomly chosen from one of the parent's sizes.
 * @param p1 The first parent individual.
 * @param p2 The second parent individual.
 * @param optional_datas Optional pointer to an integer specifying the number of crossover points to use.
 * @return The resulting individual after performing probabilistic crossover.
*/
Individual probalistic_crossover(Individual p1, Individual p2, void *optional_datas){
    int p1_len, p2_len;
    p1_len = p1.size;
    p2_len = p2.size;

    int child_len = rand() % 2 ? p1_len : p2_len;
    double probabilities_array[child_len];
    for (int i = 0; i < child_len; i++) {
        probabilities_array[i] = ((double)rand() / RAND_MAX);
    }
    Individual child;
    child.genome = malloc(sizeof(Gene) * (child_len + 1));
    for (int i = 0; i < child_len; i++) {
        if (i >= p1_len) {
            child.genome[i] = (rand() / RAND_MAX >= probabilities_array[i]) ? p2.genome[i] : create_gene();  // Append the selected gene to new_individual
        }
        else if (i >= p2_len) {
            child.genome[i] = (rand() / RAND_MAX >= probabilities_array[i]) ? p1.genome[i] : create_gene();  // Append the selected gene to new_individual
        }
        else {
            child.genome[i] = (rand() / RAND_MAX >= probabilities_array[i]) ? p1.genome[i] : p2.genome[i];  // Append the selected gene to new_individual
        }
    }

    // Return the newly created individual after crossover
    child.genome[child_len] = '\0';
    child.min_size = p1.min_size;
    child.max_size = p1.max_size;
    child.size = child_len;

    return child;
}