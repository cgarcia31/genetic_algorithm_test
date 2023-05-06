#define _GNU_SOURCE
#include <string.h>
#include <time.h>
#include <stdio.h>

#include <fitness.h>
#include <selection.h>
#include <parents.h>
#include <crossover.h>
#include <mutation.h>
#include <population.h>

int main(){
    FitnessFunction ff[9];
    ff[0] = modified_hamming_distance_fitness;
    ff[1] = levenstein_distance_fitness;
    ff[2] = smith_waterman;
    ff[3] = jaccard_similarity_fitness;
    ff[4] = cosine_similarity_fitness;
    ff[5] = pearson_correlation_fitness;
    ff[6] = nlcs_fitness;
    ff[7] = ngram_overlap_fitness;
    ff[8] = manhattan_distance_fitness;
    SelectionFunction sf[4];
    sf[0] = truncation_selection;
    sf[1] = roulette_wheel_selection;
    sf[2] = tournament_selection;
    sf[3] = rank_based_selection;
    PairingFunction pf[3];
    pf[0] = random_pairing_parents;
    pf[1] = consecutive_pairing_parents;
    pf[2] = non_sequential_pairing_parents;
    CrossoverFunction cf[3];
    cf[0] = uniform_crossover;
    cf[1] = multipoint_crossover;
    cf[2] = probalistic_crossover;
    MutationFunction mf[5];
    mf[0] = random_mutate;
    mf[1] = subsequence_inversion_mutate;
    mf[2] = swap_mutate;
    mf[3] = insertion_mutate;
    mf[4] = deletion_mutate;
    const char * word = "Je vais bien, tu vas bien !";

    int max_generations = 200000;

    int population_size = 1024;
    int min_individual_size = 2;
    int max_individual_size = 50;
    Population p = create_population(population_size, min_individual_size, max_individual_size);

    float mutation_rate = 1/strlen(word);
    float selection_rate = 0.8f;    float mutation_rate = 1/strlen(word);
    float selection_rate = 0.8f;
    int rand_fitness, rand_mutation, rand_selection, rand_pairing, rand_crossover;
    srand(time(NULL));
    int i = 0;
    for (i = 0; i < max_generations; i++){
        rand_fitness = rand()%9;
        rand_selection = rand()%4;
        rand_mutation = rand()%5;
        rand_pairing = rand()%3;
        rand_crossover = rand()%3;
        p = make_generation(p, word, ff[rand_fitness], NULL, sf[rand_selection],&selection_rate, pf[rand_pairing], NULL, cf[rand_crossover], NULL, mf[rand_mutation],NULL);
        float * fitness_scores = malloc(sizeof(float)* p.size);

        for(int j = 0; j < p.size; j++)
            fitness_scores[j] = modified_hamming_distance_fitness(p.individuals[j].genome, word, NULL);

        int * selected_indices;
        selected_indices = truncation_selection(p, fitness_scores, selection_rate, NULL);
//        printf("%s\n",p.individuals[selected_indices[0]].genome);
        if (fitness_scores[selected_indices[0]] == 0){
//            printf("Number of generation : %d\n", p.generation);
            free(selected_indices);
            free(fitness_scores);
            break;
        }
        free(selected_indices);
        free(fitness_scores);
    }
    free_population(p);
//    printf("%s : %d generations\n", p.individuals[0].genome, p.generation);
}
