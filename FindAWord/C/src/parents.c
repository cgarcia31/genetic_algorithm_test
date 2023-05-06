#include <parents.h>

/**
 * @brief Randomly pairs selected individuals.
 * Given a list of selected individuals, this function creates random pairs of individuals by 
 * randomly selecting two distinct indicesfrom the list of selected indices. 
 * The selected indices are marked as -1 to prevent them from being selected again in the next iteration.
 * @param selected_indices List of indices of selected individuals.
 * @param selected_size Number of selected individuals.
 * @param optional_datas Unused parameter.
 * @return List of random pairs of individuals.
*/
Parents * random_pairing_parents(int *selected_indices, int selected_size, void *optional_datas){
    int nb = 0, i;
    int parents_size = selected_size>>1;
    if (parents_size == 0 || selected_indices == NULL || selected_size == 0)
        return NULL;
    Parents *parents = malloc(sizeof(Parents)*parents_size);

    // Iterate through the selected indices and create random pairs.
    while (nb < selected_size - 1) {
        // Randomly select two distinct indices from the list of selected indices.
        int p[2];
        for (i = 0; i < 2; i++) {
            int index;
            do {
                index = rand() % selected_size;
            } while (selected_indices[index] == -1);
            p[i] = index;
            selected_indices[index] = -1;

        }
        parents[nb >>1].p1 = p[0];
        parents[nb >>1].p2 = p[1];
        nb += 2;
    }

    // Return the list of pairs.
    return parents;
}

/**
 * @brief Generates pairs of parents for a genetic algorithm using consecutive pairing.
 * This function takes an array of selected indices and creates pairs of consecutive indices, i.e., pairs
 * where the indices are adjacent in the input array.
 * @param selected_indices An array of selected indices to create pairs from.
 * @param selected_size The size of the selected_indices array.
 * @param optional_datas Optional data to be passed to the function. This parameter is not used in this function.
 * @return A pointer to an array of Parents structs, containing the pairs of parent indices.
 * @note The returned pointer must be freed by the caller.
*/
Parents* consecutive_pairing_parents(int *selected_indices, int selected_size, void *optional_datas) {
    int parents_size = selected_size>>1;
    Parents *parents = malloc(sizeof(Parents)*parents_size);
    int nb = 0;

    // Iterate through the selected indices and create pairs of consecutive indices.
    for (int i = 0; i < selected_size-1; i+=2) {
        parents[nb].p1 = selected_indices[i];
        parents[nb].p2 = selected_indices[i+1];
        nb++;
    }

    // Return the list of pairs.
    return parents;
}

/**
 * @brief Pair parents in a non-sequential manner.
 * Given a list of selected indices, this function pairs parents in a non-sequential manner.
 * It pairs each index with the second next index, except for the last two indices, which are paired last.
 * @param selected_indices an array of selected indices
 * @param selected_size the size of the array of selected indices
 * @param optional_datas optional parameters that may be needed for the pairing
 * @return an array of paired parents
*/
Parents* non_sequential_pairing_parents(int *selected_indices, int selected_size, void *optional_datas) {
    int parents_size = selected_size>>1;
    Parents *parents = malloc(sizeof(Parents)*parents_size);
    int i, j, pair_index = 0;
    
    // Loop over the selected indices, except the last two
    for (i = 0; i < selected_size-3; i += 2) {
        // Pair each index with the second next index
        parents[pair_index].p1 = selected_indices[i];
        parents[pair_index].p2 = selected_indices[i+2];
        pair_index++;
    }

    // Add the last pair of indices
    parents[pair_index].p1 = selected_indices[selected_size-3];
    parents[pair_index].p2 = selected_indices[selected_size-1];

    // Return the parents
    return parents;
}