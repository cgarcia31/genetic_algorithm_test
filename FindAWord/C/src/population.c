#include <population.h>

/**
 * @brief Creates a new population of individuals with the given size and range of sizes for each individual.
 * @param size The number of individuals in the population.
 * @param min_size_individual The minimum size of each individual.
 * @param max_size_individual The maximum size of each individual.
 * @return Population A new population of individuals with the given size and range of sizes for each individual.
*/
Population create_population(int size, int min_size_individual, int max_size_individual){
    Population population;
    if (size != 0){
        population.individuals = malloc(size*sizeof(Individual));
        if (population.individuals != NULL){
            for(int i = 0; i < size; i++)
                population.individuals[i] = create_individual(min_size_individual, max_size_individual);
        }
    }
        population.min_individual_size = min_size_individual;
        population.max_individual_size = max_size_individual;
        population.size = size;
        population.generation = 0;
    return population;
}

/**
 * @brief Frees the memory used by a population.
 * This function frees the memory used by a population and its individuals.
 * @param p The population to free.
*/
void free_population(Population p){
    if (p.size != 0 && p.individuals != NULL){
        for (int i = 0; i < p.size; i++){
            free_individual(p.individuals[i]);
        }
        free(p.individuals);
        p.size = 0;
    }
}

/**
 * @brief Creates a new generation of individuals from a given population.
 * @param p The population to generate the new generation from.
 * @param word The target word to evolve towards.
 * @param fitness_function The fitness function to use to evaluate individuals.
 * @param fitness_optional_datas Optional data to be passed to the fitness function.
 * @param selection_function The selection function to use to select parents for reproduction.
 * @param selection_optional_datas Optional data to be passed to the selection function.
 * @param pairing_function The pairing function to use to select pairs of parents for crossover.
 * @param pairing_optional_datas Optional data to be passed to the pairing function.
 * @param crossover_function The crossover function to use to produce children from pairs of parents.
 * @param crossover_optional_datas Optional data to be passed to the crossover function.
 * @param mutation_function The mutation function to use to mutate children.
 * @param mutation_optional_datas Optional data to be passed to the mutation function.
 * @return The new generation of individuals.
*/
Population make_generation(Population p, const char * word, FitnessFunction fitness_function, void *fitness_optional_datas, \
                            SelectionFunction selection_function, void *selection_optional_datas,\
                            PairingFunction pairing_function, void * pairing_optional_datas,\
                            CrossoverFunction crossover_function, void * crossover_optional_datas,\
                            MutationFunction mutation_function, void * mutation_optional_datas){
    Individual *children = NULL;
    int * selected_indices;
    int population_size = p.size;
    float selection_rate = 0.4f;
    float elitism_selection_rate = 0.4f;

    if (population_size == 0)
        return p;

    /* Allocate memory for new individuals*/
    Individual * new_individuals = malloc(sizeof(Individual)*population_size);

    /* Get fitness _scores for all individuals population*/
    float * fitness_scores = malloc(sizeof(float)* population_size);
    // If issues when allocating fitness_scores return p
    if (fitness_scores == NULL) 
        return p;

    for(int i = 0; i < population_size; i++)
        fitness_scores[i] = fitness_function(p.individuals[i].genome, word, fitness_optional_datas);

    // Get selected indices with truncation selection for elistism selection
    selected_indices = truncation_selection(p, fitness_scores, elitism_selection_rate, selection_optional_datas);
    int selected_size = (int) (elitism_selection_rate * population_size);

    if (selected_size != 0 && selected_indices != NULL){
        for(int i = 0; i < selected_size; i++){
            new_individuals[i].genome = malloc(sizeof(Gene) * (p.individuals[selected_indices[i]].size+1));
            new_individuals[i].size = p.individuals[selected_indices[i]].size;
            new_individuals[i].max_size = p.max_individual_size;
            new_individuals[i].min_size = p.min_individual_size;
            memcpy(new_individuals[i].genome, p.individuals[selected_indices[i]].genome, sizeof(Gene) * p.individuals[selected_indices[i]].size);
            new_individuals[i].genome[new_individuals[i].size] = '\0';
        }
    }
    if(selected_indices != NULL)
        free(selected_indices);

    int new_population_size = selected_size;

    /* Get Selection */
    selected_size = (int) (selection_rate * population_size);
    selected_indices = selection_function(p, fitness_scores, selection_rate, selection_optional_datas);

    /* We can free fitness_scores*/
    if (fitness_scores != NULL){
        free(fitness_scores);
    }

    if (selected_indices != NULL && selected_size != 0){
        /* Get Parents */
        Parents *parents;
        parents = pairing_function(selected_indices, selected_size, pairing_optional_datas);
        /* We can free selected_indices */
        if (selected_indices != NULL){
            free(selected_indices);
        }
        if (parents != NULL){
            int parents_size = selected_size >> 1;
            int number_of_child = parents_size;

            Individual *children = malloc(sizeof(Individual)*number_of_child);

            for(int i = 0; i < number_of_child; i++){
                children[i] = crossover_function(p.individuals[parents[i].p1],p.individuals[parents[i].p2], crossover_optional_datas);
                children[i] = mutation_function(children[i], mutation_optional_datas);
            }
            // Add children in new individuals
            for(int i = new_population_size; i < new_population_size + number_of_child; i++){
                new_individuals[i].genome = malloc(sizeof(Gene) * (children[i-new_population_size].size+1));
                new_individuals[i].size = children[i-new_population_size].size;
                new_individuals[i].max_size = p.max_individual_size;
                new_individuals[i].min_size = p.min_individual_size;
                memcpy(new_individuals[i].genome, children[i-new_population_size].genome, sizeof(Gene) * (children[i-new_population_size].size+1));
                new_individuals[i].genome[new_individuals[i].size] = '\0';
            }
            new_population_size += number_of_child;
            // All children are copied we can free children
            if (children != NULL){
                for (int i = 0; i < number_of_child ; i++){
                    free_individual(children[i]);
                }
                free(children);
            }
            if (parents != NULL && parents_size != 0){
                free(parents);
            }
        }
    }
    // Fill pop
    for (int i = new_population_size; i < p.size; i++){
        new_individuals[i] = create_individual(p.min_individual_size, p.max_individual_size);
    }
    new_population_size = p.size;
    free_population(p);
    p.individuals = new_individuals;
    p.size = new_population_size;
    p.generation++;
    return p;
}
