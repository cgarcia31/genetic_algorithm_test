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

// Swap two characters in a string
void swapc(char *a, char *b) {
    char temp = *a;
    *a = *b;
    *b = temp;
}
// Partition function for QuickSort
int partition(char *str, int low, int high) {
    char pivot = str[high];
    int i = (low - 1);
 
    for (int j = low; j < high; j++) {
        if (str[j] < pivot) {
            i++;
            swapc(&str[i], &str[j]);
        }
    }
    swapc(&str[i + 1], &str[high]);
    return (i + 1);
}

// QuickSort algorithm for sorting a string
void quickSort(char *str, int low, int high) {
    if (low < high) {
        int pi = partition(str, low, high);
        quickSort(str, low, pi - 1);
        quickSort(str, pi + 1, high);
    }
}

char * to_set(const char *str){
    int len_str = strlen(str);
    char * str_cpy = (char *) malloc(len_str+1);
    memcpy(str_cpy, str, len_str);
    quickSort(str_cpy, 0, len_str-1);
    char *set_str = (char *) malloc(len_str+1);
    int len = 0;
    int len2 = 0;
    while (len < len_str){
        char _char = str_cpy[len];
        if (len == 0 || _char != str_cpy[len-1]) {
            set_str[len2++] = _char;
        }
        len++;
    }
    set_str[len2] = '\0';
    return set_str;
}

char * union_set(char *set1, char *set2){
    char *union_set1 = to_set(set1);
    char *union_set2 = to_set(set2);

    int len_set1 = strlen(union_set1);
    int len_set2 = strlen(union_set2);

    char * _union = (char *) malloc (len_set1+len_set2+1);
    memcpy(_union, union_set1, len_set1);
    memcpy(_union+len_set1, union_set2, len_set2+1);

    free(union_set1);
    free(union_set2);

    return to_set(_union);
}

char * intersection(char * set1, char *set2){
    int len_set1 = strlen(set1);
    int len_set2 = strlen(set2);
    char * intersect = (char *) malloc (len_set1+len_set2+1);
    int len_intersect = 0;
    for (int i = 0; i < len_set1; i++)
        for (int j = 0; j < len_set2; j++)
            if (set1[i] == set2[j]){
                intersect[len_intersect++] = set2[j];
                break;
            }
    intersect[len_intersect] ='\0';
    return intersect;
}       

float jaccard_similarity_fitness(const char *individual, const char *word){
    int diff_len = abs(strlen(individual) - strlen(word));
    
    char * individual_set = to_set(individual);
    char * word_set = to_set(word);

    int len_union_set = strlen(to_set(union_set(individual_set, word_set)));
    int len_intersection_set = strlen(intersection(individual_set, word_set));

    float jaccard_similarity = (float)len_intersection_set / (len_union_set + diff_len);
    return 1 - jaccard_similarity;
}