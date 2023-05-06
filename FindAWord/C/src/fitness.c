#include <fitness.h>

/**
 * @brief Computes the length of a null-terminated string.
 * This function takes a null-terminated string and computes its length by iterating through the string
 * until it reaches the null character '\0'. The length is defined as the number of characters before the null
 * character.
 * Complexity : O(n), n : str length
 * @param str A pointer to the null-terminated string whose length is to be computed.
 * @return The length of the null-terminated string.
*/
inline int len(const char * str){
    int i = 0;
    while(str[i] != '\0'){
        i++; 
    }
    return i;
}

/**
 * 
 * @brief Creates a new string that contains only the unique characters in the input string.
 * This function creates a new string that contains only the unique characters in the input string.
 * The input string is scanned character by character, and each unique character is added to the output string.
 * The output string is terminated by a null character.
 * @param str The input string to extract unique characters from.
 * @return A pointer to the new string containing only the unique characters from the input string.
 * @note The returned string must be freed with free() when it is no longer needed.
*/
char* to_set(const char* str) {
    int str_len = len(str);
    const char * end_point = str + str_len;

    char *set = malloc(str_len + 1);
    char *save_set = set;
    int len = 0;

    char seen[256] = {0};  // initialize all elements to 0

    while (str < end_point){
        if (!seen[*str]){
            seen[*str] = 1;
            *set++ = *str;
        }
        str++;
    }
    *set = '\0';
    return save_set;
}

/**
 * 
 * @brief Computes the union of two sets.
 * This function takes two sets and returns their union, which is a set containing all the elements from both sets
 * Without duplicates. The resulting set is returned as a dynamically allocated string.
 *
 * @param set1 The first set to be included in the union.
 * @param set2 The second set to be included in the union.
 *
 * @return A dynamically allocated string containing the union of the two sets.
 */
char* union_set(const char* set1, const char* set2) {
    int set1_len = len(set1);
    int set2_len = len(set2);

    char *unionset = malloc(set1_len + set2_len + 1);
    char *save_unionset = unionset;

    char seen[256] = {0};  // initialize all elements to 0

    while (*set1 != '\0'){
        if (!seen[*set1]){
            seen[*set1] = 1;
            *unionset++ = *set1;
        }
        set1++;
    }

    while (*set2 != '\0'){
        if (!seen[*set2]){
            seen[*set2] = 1;
            *unionset++ = *set2;
        }
        set2++;
    }

    *unionset = '\0';

    return save_unionset;
}

/**
 * 
 * @brief Computes the intersection of two sets.
 * This function takes two input sets and returns their intersection. The resulting set contains only
 * elements that are in both input sets.
 * 
 * @param set1 The first input set.
 * @param set2 The second input set.
 * @return A new string containing the intersection of the two input sets.
 */
char* intersection_set(const char* set1, const char* set2) {
    int set1_len = len(set1);
    int set2_len = len(set2);

    int min_len = set1_len <  set2_len ? set1_len : set2_len;
    char *intersection_set = malloc(min_len + 1);
    char *save_intersection_set = intersection_set;

    char seen1[256] = {0};  // initialize all elements to 0
    char seen2[256] = {0};  // initialize all elements to 0

    while (*set1 != '\0'){
        if (!seen1[*set1]){
            seen1[*set1] = 1;
        }
        set1++;
    }
    while (*set2 != '\0'){
        if (!seen2[*set2]){
            seen2[*set2] = 1;
        }
        set2++;
    }

    for (int i = 0; i< 256; i++){
        if (seen1[i] & seen2[i]){
            *intersection_set++ = i;
        }
    }

    *intersection_set = '\0';

    return save_intersection_set;
}

/**
 * @brief Calculates the modified Hamming distance between two strings and returns it as a fitness score.
 *
 * This function calculates the modified Hamming distance between the two input strings, where the modified Hamming distance
 * is defined as the number of characters that differ between the two strings, plus the absolute difference between the lengths
 * of the two strings. The resulting distance is then normalized by the length of the longer string to obtain a fitness score
 * in the range [0, 1].
 * Complexity :  O(min(m,n)) which m,n are strings length
 * @param individual The first string.
 * @param word The second string.
 * @param optional_datas Optional parameters for the function.
 * @return The fitness score, where a value of 0 represents a perfect match and a value of 1 represents a complete mismatch.
 */
float modified_hamming_distance_fitness(const char *individual, const char *word, void * optional_datas) {
    int n, m, sum, min_mn, max_mn, i;
    const char *s1, *s2;

    // Calculate the length of each string
    n = len(individual);
    m = len(word);

    // Determine the shorter string
    if (m < n){ 
        min_mn = m; 
        max_mn = n;
        s1 = word; 
        s2 = individual;
    }else{
        min_mn = n;
        max_mn = m;
        s1 = individual;
        s2 = word;
    }

    // If the lengths are different, add the difference to the sum
    if (max_mn != min_mn){ 
        sum = max_mn - min_mn; 
    }else{
        sum = 0;
    }

    // Compute the Hamming distance between the two strings
    for (i = 0; i < min_mn; i++)
        if (*(s1++) != *(s2++))
            sum++;

    // Normalize the distance by the length of the longer string
    return (float)sum / max_mn;
}

/**
 * @brief Calculate the Levenshtein distance between two strings and return the fitness value.
 * Complexity O(mn)
 *
 * @param individual The first string to compare.
 * @param word The second string to compare.
 * @param optional_datas Optional data that can be passed to the function.
 * @return The fitness value as a floating point number.
 */
float levenstein_distance_fitness(const char *individual, const char *word, void *optional_datas) {
    int n, m, max_len, i, j , i_pred, j_pred, cost, pred_cost;
    n = len(individual)+1;
    m = len(word)+1;
    max_len = n > m ? n : m;

    int distances[n][m];

    memset(distances, 0, sizeof(distances));
    for (i = 0; i < max_len; i++) {
        if (i < n )
            distances[i][0] = i;
        if (i < m)
            distances[0][i] = i;
    }

    for (i = 1, i_pred = 0; i < n; i++, i_pred++) {
        for (j = 1, j_pred = 0; j < m; j++, j_pred++) {
            cost = (individual[i_pred] != word[j_pred]);
            pred_cost = distances[i_pred][j_pred] + cost;
            distances[i][j] = distances[i_pred][j] < distances[i][j_pred] ? distances[i_pred][j] : distances[i][j_pred];
            distances[i][j]++;
            if (distances[i][j] > pred_cost){
                distances[i][j] = pred_cost;
            }
        }
    }
    int m_len = max_len == 1 ? 1 : max_len -1;
    return (float)(distances[n-1][m-1]) / m_len;
}

/**
 * @brief Computes the Smith-Waterman similarity score between two strings.
 * 
 * The Smith-Waterman algorithm is a dynamic programming algorithm that computes 
 * the similarity between two strings by finding the optimal local alignment 
 * between them. This function implements the algorithm with a match score of 2, 
 * a mismatch score of -1, and a gap penalty of -1. The complexity of the 
 * algorithm is O(mn), where m and n are the lengths of the two input strings.
 * 
 * @param individual The first input string.
 * @param word The second input string.
 * @param optional_datas An optional pointer to additional data that can be used 
 *                            by the function (not used in this implementation).
 * @return The Smith-Waterman similarity score between the two input strings, 
 *         normalized by the maximum possible score.
 * @retval -1.0f if an error occurred during memory allocation for the score matrix.
 */
float smith_waterman(const char *individual, const char *word, void * optional_datas) {
    int match_score = 2, mismatch_score = -1, gap_penalty = -1;
    int n, m, max_mn, max_score = 0, i, j, match, upper_left, max_value, diagonal, intermediate_calc;

    // Calculate length of the two strings
    n = len(individual);
    m = len(word);

    // Determine the maximum length between the two strings
    max_mn = n > m ? n : m;

    // Increase m to account for the initialization of score_matrix
    m++;

    // Allocate memory for score_matrix
    int *score_matrix = calloc(m, sizeof(int));
    if (score_matrix == NULL) {
        return -1.0f;
    }

    // Populate score_matrix
    for (i = 1; i < n + 1; i++) {
        diagonal = 0;
        for (j = 1; j < m; j++) {
            // Calculate match score
            match = (individual[i-1] == word[j-1]) ? match_score : mismatch_score;

            // Store the value of score_matrix[j] for next iteration
            upper_left = diagonal;

            // Store the value of score_matrix[j-1] for next iteration
            diagonal = score_matrix[j];

            // Calculate the three values that are candidates for the next value of score_matrix[j]
            intermediate_calc = upper_left + match;
            max_value = intermediate_calc > 0 ? intermediate_calc : 0;
            intermediate_calc = score_matrix[j] + gap_penalty;
            max_value = max_value > intermediate_calc ? max_value : intermediate_calc;
            intermediate_calc = score_matrix[j-1] + gap_penalty;

            // Choose the maximum value as the new value of score_matrix[j]
            score_matrix[j] = max_value > intermediate_calc ? max_value : intermediate_calc;

            // Keep track of the maximum value seen so far
            if (score_matrix[j] > max_score) {
                max_score = score_matrix[j];
            }
        }
    }
    
    // Free memory allocated for score_matrix
    free(score_matrix);

    // Return the Smith-Waterman similarity score
    return 1.0f - (float)(max_score) / (max_mn << 1);
}

/**
 * @brief Calculates the fitness score of an individual string using Jaccard similarity distance
 * compared to a target word.
 * @param individual The individual string to evaluate.
 * @param word The target word to compare against.
 * @param optional_datas Optional data that can be used in the evaluation process.
 * @return The fitness score of the individual string, where a score of 1 indicates an exact match
 *  with the target word, and lower scores indicate greater distance from the target word.
*/
float jaccard_similarity_fitness(const char *individual, const char *word, void * optional_datas){
    // Calculate the length of the difference between the two strings
    int diff_len = abs(len(individual) - len(word));
    
    // Convert the strings to sets and calculate the union and intersection sets
    char * individual_set = to_set(individual);
    char * word_set = to_set(word);
    char * union_set_str = union_set(individual_set, word_set);
    char * intersection_set_str = intersection_set(individual_set, word_set);

    // Calculate the length of the union and intersection sets
    int len_union_set = len(union_set_str);
    int len_intersection_set = len(intersection_set_str);

    // Calculate the Jaccard similarity distance
    float jaccard_similarity = (float)len_intersection_set / (len_union_set + diff_len);
    
    // Free the dynamically allocated memory used for the sets and their union/intersection
    free(individual_set);
    free(word_set);
    free(union_set_str);
    free(intersection_set_str);

    return 1 - jaccard_similarity;
}

/**
 * @brief Calculates the fitness score of an individual using the Normalized Longest Common Subsequence (NLCS) algorithm.
 *
 * The NLCS algorithm measures the similarity between two strings by calculating their longest common subsequence (LCS) and 
 * normalizing it by the length of the longer string.
 *
 * @param[in] individual The first string to compare.
 * @param[in] word The second string to compare.
 * @param[in] optional_datas Optional data that can be passed to the function. Not used in this implementation.
 * 
 * @return The fitness score of the individual. The score is a float between 0 and 1, where 1 means that the two strings are 
 *         identical and 0 means that they have no common characters.
 */
float nlcs_fitness(const char * individual, const char * word, void * optional_datas){
    int n = len(individual);
    int m = len(word);
    int max_mn = n > m ? n : m;

    int match_matrix[n][m];

    // Init match_matrix
    for (int i = 0; i < n; i++){
        for (int j = 0; j < m ; j++){
            match_matrix[i][j] = (individual[i] == word[j]);
        }
    }

    // Calculate LCS
    int lcs_length = 0;
    for (int i = 1; i < n; i++){
        for (int j = 1; j < m; j++){
            if (match_matrix[i-1][j-1]){
                match_matrix[i][j] = match_matrix[i-1][j-1] + 1;
            }
            else{
                if (match_matrix[i][j] < match_matrix[i-1][j-1]){
                    match_matrix[i][j] = match_matrix[i-1][j-1];
                }
            }
            if (lcs_length < match_matrix[i][j]){
                lcs_length = match_matrix[i][j];
            }
        }
    }
    
    // Return 1 - Normalize LCS
    return 1 - (float) lcs_length / max_mn;
}

/**
 * @brief Computes the cosine similarity fitness score between two strings represented as character arrays.
 * The smaller vector is padded with zeros to match the length of the larger vector.
 * @param individual A character array representing the first string.
 * @param word A character array representing the second string.
 * @param optional_datas Optional data pointer that can be used to pass additional parameters.
 * @return A floating point value representing the cosine similarity fitness score.
*/
float cosine_similarity_fitness(const char* individual, const char* word, void * optional_datas) {
    // Pad the smaller vector with zeros
    int n = len(individual);
    int m = len(word);
    int max_len = n > m ? n : m;
    char padded_vec1[max_len];
    char padded_vec2[max_len];

    for (int i = 0; i < max_len; i++) {
        padded_vec1[i] = i < n ? individual[i] : 0;
        padded_vec2[i] = i < m ? word[i] : 0;
    }

    // Compute dot product
    int dot_product = 0;
    for (int i = 0; i < max_len; i++) {
        dot_product += padded_vec1[i] * padded_vec2[i];
    }

    // Compute magnitudes
    long sum1 = 0;
    long sum2 = 0;
    for (int i = 0; i < max_len; i++) {
        sum1 += padded_vec1[i] * padded_vec1[i];
        sum2 += padded_vec2[i] * padded_vec2[i];
    }
    float magnitude1 = sqrt(sum1);
    float magnitude2 = sqrt(sum2);

    // Compute cosine similarity
    if (magnitude1 == 0 || magnitude2 == 0) {
        return 1;
    } else {
        float calc = dot_product / (magnitude1 * magnitude2);
        float similarity_score = 1 - (calc + 1) / 2;
        return similarity_score;
    }
}

/**
 * @brief Computes the fitness of an individual string based on the n-gram overlap score with a given word.
 * @param individual The individual string to compute the fitness of.
 * @param word The target word to compute the n-gram overlap score with.
 * @param optional_datas Optional data to be used in the fitness computation (not used in this implementation).
 * @return The n-gram overlap fitness score of the individual string.
*/
float ngram_overlap_fitness(const char* individual, const char* word, void * optional_datas) {
    int n = 2;
    int i, j, k, count1, count2, intersection, _union, len1, len2, min_len;
    char ngram1[n+1], ngram2[n+1];
    
    // Get the lengths of the two strings and determine the minimum length
    len1 = len(individual);
    len2 = len(word);
    min_len = len1 < len2 ? len1 : len2;
    
    // Make sure n is not greater than the minimum length
    n = n > min_len ? min_len : n;
    
    // Allocate memory for the n-gram arrays
    char** ngrams1 = (char**) malloc(sizeof(char*) * (len1 - n + 1));
    char** ngrams2 = (char**) malloc(sizeof(char*) * (len2 - n + 1));
    for (i = 0; i < len1 - n + 1; i++) {
        ngrams1[i] = (char*) malloc(sizeof(char) * (n+1));
        strncpy(ngrams1[i], &individual[i], n);
        ngrams1[i][n] = '\0';
    }
    for (i = 0; i < len2 - n + 1; i++) {
        ngrams2[i] = (char*) malloc(sizeof(char) * (n+1));
        strncpy(ngrams2[i], &word[i], n);
        ngrams2[i][n] = '\0';
    }
    
    // Compute the intersection of the two sets of n-grams
    intersection = 0;
    for (i = 0; i < len1 - n + 1; i++) {
        for (j = 0; j < len2 - n + 1; j++) {
            if (strcmp(ngrams1[i], ngrams2[j]) == 0) {
                intersection++;
                break;
            }
        }
    }
    
    // Compute the union of the two sets of n-grams
    _union = len1 - n + 1 + len2 - n + 1 - intersection;
    
    // Compute the n-gram overlap score
    float overlap_score = (float) intersection / (float) _union;
    
    // Free the memory allocated for the n-gram arrays
    for (i = 0; i < len1 - n + 1; i++) {
        free(ngrams1[i]);
    }
    for (i = 0; i < len2 - n + 1; i++) {
        free(ngrams2[i]);
    }
    free(ngrams1);
    free(ngrams2);
    
    return 1 - overlap_score;
}

/**
 * @brief Calculates the Manhattan distance between two strings, normalized by the maximum possible distance.
 * @param individual The first string to compare.
 * @param word The second string to compare.
 * @param optional_datas Unused optional data.
 * @return The Manhattan distance fitness as the inverse of the distance, normalized by the maximum possible distance.
*/
float manhattan_distance_fitness(const char* individual, const char* word, void * optional_datas) {
    int n = len(individual);
    int m = len(word);
    int min_mn = n < m ? n : m;
    int max_mn = n < m ? m : n;
    int i = 0;
    long manhattan_distance = 0;

    // Compute the Manhattan distance
    for (; i < min_mn; i++) 
        manhattan_distance += abs(individual[i] - word[i]);
    manhattan_distance += abs(m-n) * 96;
    // Compute the Manhattan distance fitness as the inverse of the distance
    float manhattan_distance_fitness = manhattan_distance /(max_mn * 96.0f); // 96 is the maximum distance between two characters in the model

    return manhattan_distance_fitness;
}

/**
 * 
 * @brief Calculates the fitness of an individual string based on the Pearson correlation with a given word
 * This function calculates the fitness of an individual string by computing its Pearson correlation with a given word.
 * The Pearson correlation is a measure of the linear relationship between two variables. In this case, the variables
 * are the characters in the two strings. The Pearson correlation is a value between -1 and 1, with values closer to 1
 * indicating a stronger positive correlation and values closer to -1 indicating a stronger negative correlation.
 * @param individual The individual string whose fitness is being calculated
 * @param word The word to which the individual string is being compared
 * @param optional_datas Optional additional data needed for the fitness calculation (not used in this implementation)
 * @return The fitness of the individual string, expressed as a float between 0 and 1. A value of 0 indicates no correlation
 * between the two strings, while a value of 1 indicates a perfect correlation.
*/
float pearson_correlation_fitness(const char* individual, const char* word, void * optional_datas) {
    int n = len(individual);
    int m = len(word);
    int min_mn = n < m ? n : m;

    // Compute the mean of the characters in the strings
    float mean_individual = 0.0f;
    float mean_word = 0.0f;
    for (int i = 0; i < n; i++) {
        mean_individual += individual[i];
    }
    mean_individual /= (float) n;

    for (int j = 0; j < m; j++) {
        mean_word += word[j];
    }
    mean_word /= (float) m;

    // Compute the numerator and denominator of the Pearson correlation
    float numerator = 0.0;
    float denominator_individual = 0.0;
    float denominator_word = 0.0;

    for (int i = 0; i < min_mn; i++) {
        float x = (float) individual[i] - mean_individual;
        float y = (float) word[i] - mean_word;
        numerator += x * y;
        denominator_individual += x * x;
        denominator_word += y * y;
    }
    // If the strings are of different length, add the remaining characters to the denominator
    if (m != n){
        if (n > m){
            for (int i = m; i < n; i++){
                float x = (float) (individual[i] - mean_individual);
                denominator_individual += x * x;
            }
        }
        else{
            for (int i = n; i < m; i++){
                float y = (float) (word[i] - mean_word);
                denominator_word += y * y;
            }
        }
    }

    // Compute the Pearson correlation
    float pearson_correlation;
    if (denominator_individual == 0 || denominator_word == 0) {
        pearson_correlation = 0.0;
    } else {
        float denominator = sqrt(denominator_individual) * sqrt(denominator_word);
        pearson_correlation = numerator / denominator;
    }

    // Map the result to [0, 1]
    float mapped_result = (pearson_correlation + 1.0) / 2.0;
    return 1 - mapped_result;
}