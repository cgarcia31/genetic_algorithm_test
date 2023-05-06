#include <mutation.h>

/**
 * @brief Computes the length of a null-terminated character string.
 * @param str The string whose length is to be determined.
 * @return The length of the string, excluding the null character.
*/
inline int len(const char * str){
    const char *str2 = str; 
    int sum = 0;
    while(*str2++ != '\0')
        sum++; 
    return sum;
}

/**
 * @brief Mutates an individual's genome by randomly changing a number of genes based on the mutation rate.
 * @param c The individual to be mutated.
 * @param optional_datas Optional pointer to mutation rate. If NULL, mutation rate is calculated as 1/individual size.
 * @return The mutated individual.
*/
Individual random_mutate(Individual c, void * optional_datas){
    float mutation_rate;
    int individual_size = c.size;

    if (optional_datas != NULL){
        mutation_rate = *(float *) optional_datas;
    }else{
        mutation_rate = 1/c.size;
    }
    // Calculate the number of mutations based on mutation rate    
    int number_of_mutations = (int) (mutation_rate * individual_size);

    for (int i = 0; i < number_of_mutations; i++) {
        // Select a random gene to be mutated
        int gene_to_modify = rand() % individual_size;
        // Replace the selected gene with a randomly chosen character
        c.genome[gene_to_modify] = create_gene();
    }
    // Return the mutated individual
    return c;
}

/**
 * @brief Performs subsequence inversion mutation on an individual's genome.
 * This function randomly selects two points in the individual's genome and inverts the subsequence
 * between them. The inversion operation is performed in-place.
 * @param c The individual to mutate.
 * @param optional_datas Optional pointer to any additional data required for the mutation.
 * @return The mutated individual.
*/
Individual subsequence_inversion_mutate(Individual c, void * optional_datas){
    int individual_size = c.size;
    int i, j;
    if (individual_size % 2 != 0) {
        i = individual_size != 1 ? rand() % (individual_size - 1) : 1;
        j = individual_size -i -1 != 0 ? rand() % (individual_size - i - 1) + i + 1: 0;
    } else {
        i = individual_size != 0 ? rand() % individual_size : 1;
        j = individual_size -i != 0 ? rand() % (individual_size - i) + i: 0;
    }

    char temp;
    while (i < j) {
        temp = c.genome[i];
        c.genome[i] = c.genome[j];
        c.genome[j] = temp;
        i++;
        j--;
    }

    return c;
}

/**
 * @brief Perform swap mutation on an individual with a given mutation rate
 * @param c The individual to be mutated
 * @param optional_datas Optional data to be used for mutation rate. If not provided, the default mutation rate of 0.2 will be used.
 * @return The mutated individual
*/
Individual swap_mutate(Individual c, void * optional_datas){
    int individual_size = c.size;
    float mutation_rate;
    if (optional_datas != NULL){
        mutation_rate = *(float *)optional_datas;
    }else{
        mutation_rate = 0.2f;
    }
    int number_of_swap = (int) (mutation_rate * individual_size);
    for (int i = 0; i < number_of_swap; i++){
        // Select two distinct indices randomly
        int i, j;
        do {
            i = rand() % individual_size;
            j = rand() % individual_size;
        } while (i == j);

        // Swap the values at the selected indices and return the mutated individual
        int temp = c.genome[i];
        c.genome[i] = c.genome[j];
        c.genome[j] = temp;
    }

    return c;
}

/**
 * @brief Mutates an individual by inserting a randomly generated gene at a random position in the genome
 * @param c The individual to mutate
 * @param optional_datas A pointer to optional parameters to control mutation rate
 * @return Individual The mutated individual
 * This function mutates an individual by inserting a randomly generated gene at a random position
 * in the genome. The mutation rate is determined by the optional parameter mutation_rate, which
 * is a pointer to a float. If mutation_rate is not provided, the default value of 0.2 is used. If
 * the mutation would result in an individual that exceeds the maximum allowable size, the original
 * individual is returned unchanged.
*/
Individual insertion_mutate(Individual c, void * optional_datas){
    int max_individual_size = c.max_size;
    int min_individual_size = c.min_size;
    int individual_size = c.size;
    if (individual_size + 1 < c.max_size){
        /* Select random index to insert gene */
        int index = rand() % (individual_size + 1);
        /* Choose a random gene to insert */
        Gene gene = create_gene();
        /* Create a new string with the inserted gene */
        Individual new_individual;
        new_individual.genome = calloc(c.size + 2, sizeof(Gene));
        strncpy(new_individual.genome, c.genome, index);
        new_individual.genome[index] = gene;
        strcpy(&new_individual.genome[index+1], &c.genome[index]);
        /* Free memory allocated for the old individual and return the new individual */
        new_individual.max_size = c.max_size;
        new_individual.min_size = c.min_size;
        new_individual.size = c.size+1;
        new_individual.genome[new_individual.size] = '\0';
        free_individual(c);
        return new_individual;
    }
    return c;
}

/**
 * @brief Mutates an individual by deleting a random gene from its genome.
 * This function deletes a random gene from an individual's genome to create a new mutated individual.
 * @param c The individual to be mutated.
 * @param optional_datas A pointer to optional parameters. In this case, it is not used.
 * @return The mutated individual.
*/
Individual deletion_mutate(Individual c, void * optional_datas){
    int max_individual_size = c.max_size;
    int min_individual_size = c.min_size;
    int individual_size = c.size;
    if (individual_size -1 >= min_individual_size){
        /* Select random index to delete gene */
        int index = rand() % individual_size;
        /* Create a new string with the deleted gene */
        Individual new_individual;
        new_individual.genome = (char*) malloc((c.size) * sizeof(Gene));
        memcpy(new_individual.genome, c.genome, index * sizeof(Gene));
        memcpy(&new_individual.genome[index], &c.genome[index+1], (c.size - index - 1) * sizeof(Gene));
        /* Free memory allocated for the old individual and return the new individual */
        new_individual.max_size = c.max_size;
        new_individual.min_size = c.min_size;
        new_individual.size = c.size-1;
        new_individual.genome[new_individual.size] = '\0';
        free_individual(c);
        return new_individual;
    }
    return c;
}
