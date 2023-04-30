#include <string.h>
#include <stdlib.h>

#define swap(x, y) do { \
    int temp_swap = x; \
    x = y; \
    y = temp_swap; \
} while (0)

int max(int num1, int num2){
    return (num1 > num2 ) ? num1 : num2;
}

int min(int num1, int num2) {
    return (num1 > num2 ) ? num2 : num1;
}

/* Complexity :  O(min(m,n)) */
float modified_hamming_distance_fitness(const char *individual, const char *word) {
    int min_mn = strlen(individual);
    int max_mn = strlen(word);
    if (min_mn > max_mn) {
        swap(min_mn, max_mn);
    }
    int diff_xy = max_mn - min_mn;
    int sum = diff_xy;
    
    for (int i = 0; i < min_mn; i++) {
        sum += (individual[i] != word[i]);
    }
    float score = (float)sum / max_mn;
    return score;
}

/* Complexity : O(mn)*/
float leveinstein_distance_fitness(const char *individual, const char *word){
    int m = strlen(individual);
    int n = strlen(word);
    
    int distances[m+1][n+1];
    memset(distances, 0, sizeof(distances));

    for(int i=0; i < m+1; i++)
        distances[i][0] = i;
    for(int j=0; j < n+1; j++)
        distances[0][j] = j;

    for (int i=1; i < m+1; i++)
        for(int j=1; j < n+1; j++)
            if (individual[i-1] == word[j-1])
                distances[i][j] = distances[i-1][j-1];
            else
                distances[i][j] = min(min(distances[i][j-1], distances[i-1][j]), distances[i-1][j-1]) + 1;
    float score = (float)(distances[m][n]) / max(m,n);
    return score;
}

/* Complexity O(mn)*/
float smith_waterman(const char *individual, const char *word, int match_score, int mismatch_score, int gap_penalty) {
    int m = strlen(individual);
    int n = strlen(word);

    int score_matrix[n+1];
    memset(score_matrix, 0, sizeof(score_matrix));

    int max_score = 0;
    for (int i = 1; i < m + 1; i++) {
        int diagonal = 0;
        for (int j = 1; j < n + 1; j++) {
            int match = (individual[i-1] == word[j-1]) ? match_score : mismatch_score;
            int upper_left = diagonal;
            diagonal = score_matrix[j];
            int max_value = max(0, upper_left + match);
            max_value = max(max_value, score_matrix[j] + gap_penalty);
            score_matrix[j] = max(max_value, score_matrix[j-1] + gap_penalty);
            if (score_matrix[j] > max_score) {
                max_score = score_matrix[j];
            }
        }
    }
    float score = 1 - (float)(max_score) / (max(m, n) << 1);

    return score;
}