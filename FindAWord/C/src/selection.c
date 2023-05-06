#include <selection.h>

/**
 * @brief Compare function used by qsort to sort an array of IndividualScore by their score
 * @param a Pointer to the first IndividualScore struct to be compared
 * @param b Pointer to the second IndividualScore struct to be compared
 * @return int Returns a negative value if a should be placed before b, zero if they are equivalent,
 * and a positive value if b should be placed before a in the sorted array
*/
int score_cmp(const void* a, const void* b) {
    const IndividualScore* score_a = (const IndividualScore*)a;
    const IndividualScore* score_b = (const IndividualScore*)b;
    return (score_a->score > score_b->score) - (score_a->score < score_b->score);
}

/**
 * @brief Performs truncation selection on a population based on fitness scores and a selection rate.
 * @param p The population to perform selection on.
 * @param fitness_scores The fitness scores for each individual in the population.
 * @param selection_rate The rate at which to select individuals from the population.
 * @param optional_datas Optional additional data required for the selection method.
 * @return An array of indices of the selected individuals.
*/
int* truncation_selection(Population p, float * fitness_scores, float selection_rate, void *optional_datas) {
    int population_size = p.size;
    int selected_size = (int)(population_size * selection_rate);
    if (population_size == 0 || fitness_scores == NULL || selected_size == 0)
        return NULL;
    
    // Calculate fitness scores for each individual in the population based on the selected fitness method
    IndividualScore* fitness_scores_i = malloc(sizeof(IndividualScore) * population_size);
    for(int i = 0; i < population_size; i++) {
        fitness_scores_i[i].idx = i;
        fitness_scores_i[i].score = fitness_scores[i];
    }
    
    // Sort the scores
    qsort(fitness_scores_i, population_size, sizeof(IndividualScore), score_cmp);
    
    // Extract the indices of the sorted fitness scores
    int* ranked_indices = malloc(sizeof(int) * selected_size);
    for(int i = 0; i < selected_size; i++) {
        ranked_indices[i] = fitness_scores_i[i].idx;
    }

    // Free allocated memory
    free(fitness_scores_i);

    return ranked_indices;
}

/**
 * @brief Perform rank-based selection on the given population based on their fitness scores.
 * @param p The population to select from.
 * @param fitness_scores An array of fitness scores for each individual in the population.
 * @param selection_rate The proportion of individuals to select from the population.
 * @param optional_datas Optional data that may be used by the function (not used in this implementation).
 * @return An array of indices representing the selected individuals.
*/
int *rank_based_selection(Population p, float * fitness_scores, float selection_rate, void *optional_datas) {
    int population_size = p.size;
    
    int i, j, idx;
    int selected_size = (int)(population_size * selection_rate);
    float *selection_probabilities = (float *)malloc(sizeof(float) * population_size);
    int *selected_indices = (int *)malloc(sizeof(int) * selected_size);
    int *ranks = (int *)malloc(sizeof(int) * population_size);
    float sum_select_probabilities = 0.0f;
    float rand_val, cum_prob;

    // Calculate fitness scores for each individual in the population based on the selected fitness method
    IndividualScore* fitness_scores_i = malloc(sizeof(IndividualScore) * population_size);
    for(int i = 0; i < population_size; i++) {
        fitness_scores_i[i].idx = i;
        fitness_scores_i[i].score = fitness_scores[i];
    }
    qsort(fitness_scores_i, population_size, sizeof(IndividualScore), score_cmp);

    // Calculate the rank of each individual in the population
    for(int i = 0; i < population_size; i++) {
        ranks[fitness_scores_i[i].idx] = i;
    }
    // Calculate the selection probabilities based on rank
    for (i = 0; i < population_size; i++) {
        selection_probabilities[i] = (population_size - ranks[i])/(float)population_size;
        sum_select_probabilities += selection_probabilities[i];
    }
    for (i = 0; i < population_size; i++) {
        selection_probabilities[i] /= sum_select_probabilities;
    }
    // Select individuals based on their selection probabilities
    j = 0;
    while (j < selected_size) {
        rand_val = (float)rand() / (float)RAND_MAX;
        cum_prob = 0.0f;
        for (i = 0; i < population_size; i++) {
            cum_prob += selection_probabilities[i];
            if (cum_prob > rand_val) {
                idx = ranks[i];
                if (idx != -1) {
                    selected_indices[j++] = idx;
                    ranks[i] = -1;
                    break;
                }
            }
        }
    }
    free(selection_probabilities);
    free(ranks);
    free(fitness_scores_i);
    return selected_indices;
}

/**
 * @brief Selects individuals from the population using the truncation selection method.
 * @param p The population to select from.
 * @param fitness_scores An array of fitness scores corresponding to each individual in the population.
 * @param selection_rate The percentage of the population to select.
 * @param optional_datas Optional additional data to be used in the selection process.
 * @return An array of selected indices.
 */
int* roulette_wheel_selection(Population p, float * fitness_scores, float selection_rate, void *optional_datas) {
    int population_size = p.size;
    int i, j;
    int selected_size = (int)(population_size * selection_rate);

    // Calculate selection probabilities based on fitness scores
    float* probabilities = malloc(sizeof(float) * population_size);

    float sum_probabilities = 0.0f;
    for (i = 0; i < population_size; i++) {
        probabilities[i] = 1.0f - fitness_scores[i] + EPSILON;
        sum_probabilities += probabilities[i];
    }
    for (i = 0; i < population_size; i++) {
        probabilities[i] /= sum_probabilities;
    }

    // Randomly choose selected_size number of indices based on probabilities
    int* selected_indices = malloc(sizeof(int) * selected_size);

    for (i = 0; i < selected_size; i++) {
        float r = rand() / (float)RAND_MAX; // Generate a random number between 0 and 1
        for (j = 0; j < population_size - 1; j++) {
            if (r < probabilities[j]) {
                selected_indices[i] = j;
                break;
            }
            r -= probabilities[j];
        }
        if (j == population_size - 1) {
            selected_indices[i] = j;
        }
    }

    // Free memory and return the selected indices
    free(probabilities);
    return selected_indices;
}

/**
 * @brief individuals from the population using tournament selection.
 * @param p The population to select from.
 * @param fitness_scores The fitness scores of each individual in the population.
 * @param selection_rate The percentage of individuals to select.
 * @param optional_datas Optional data that may be used by the selection algorithm.
 * @return An array of selected indices.
*/
int* tournament_selection(Population p, float * fitness_scores, float selection_rate, void *optional_datas) {
    int tournament_size = p.size >> 2;

    int population_size = p.size;
    int selected_size = (int) (population_size * selection_rate);
    int* selected_indices = (int*) malloc(selected_size * sizeof(int));
    int i, j, k, rand_index, contestant_index, winner_index;
    float* tournament_scores = (float*) malloc(tournament_size * sizeof(float));
    for (i = 0; i < selected_size; i++) {
        // Randomly sample a subset of individuals as tournament contestants
        int* tournament_indices = (int*) malloc(tournament_size * sizeof(int));
        for (j = 0; j < tournament_size; j++) {
            rand_index = rand() % population_size;
            tournament_indices[j] = rand_index;
        }
        
        // Calculate fitness scores for each contestant in the tournament
        for (j = 0; j < tournament_size; j++) {
            contestant_index = tournament_indices[j];
            tournament_scores[j] = fitness_scores[contestant_index];
        }
        
        // Select the winner based on the highest fitness score
        winner_index = tournament_indices[0];
        for (j = 1; j < tournament_size; j++) {
            if (tournament_scores[j] < fitness_scores[winner_index]) {
                winner_index = tournament_indices[j];
            }
        }
        
        // Add the winner's index to the selected indices
        selected_indices[i] = winner_index;
        free(tournament_indices);
    }
    
    free(tournament_scores);
    return selected_indices;
}
