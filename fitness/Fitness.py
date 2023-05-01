import ctypes
import math

class Fitness():
    @staticmethod
    def load_fitness_lib(fitness_lib):
        # Load the fitness library and get the function pointers
        c_library = ctypes.CDLL(fitness_lib)
        
        # Set the modified_hamming_distance_fitness function
        Fitness.modified_hamming_distance_fitness = c_library.modified_hamming_distance_fitness
        Fitness.modified_hamming_distance_fitness.argtypes = [ctypes.c_char_p, ctypes.c_char_p]
        Fitness.modified_hamming_distance_fitness.restype = ctypes.c_float
        
        # Set the leveinstein_distance_fitness function
        Fitness.leveinstein_distance_fitness = c_library.leveinstein_distance_fitness
        Fitness.leveinstein_distance_fitness.argtypes = [ctypes.c_char_p, ctypes.c_char_p]
        Fitness.leveinstein_distance_fitness.restype = ctypes.c_float
        
        # Set the smith_waterman function
        Fitness.smith_waterman = c_library.smith_waterman
        Fitness.smith_waterman.argtypes = [ctypes.c_char_p, ctypes.c_char_p, ctypes.c_int, ctypes.c_int, ctypes.c_int]
        Fitness.smith_waterman.restype = ctypes.c_float

        # Set the jaccard_similarity_fitness function
        Fitness.jaccard_similarity_fitness = c_library.jaccard_similarity_fitness
        Fitness.jaccard_similarity_fitness.argtypes = [ctypes.c_char_p, ctypes.c_char_p]
        Fitness.jaccard_similarity_fitness.restype = ctypes.c_float

    def modified_hamming_distance_fitness_c(self, individual, word):
        # Call the modified_hamming_distance_fitness function from the library
        return Fitness.modified_hamming_distance_fitness(individual, word)

    def leveinstein_distance_fitness_c(self, individual, word):
        # Call the leveinstein_distance_fitness function from the library
        return Fitness.leveinstein_distance_fitness(individual, word)
    
    def smith_waterman_c(self, individual, word, match_score=2, mismatch_score=-1, gap_penalty=-1):
        # Call the smith_waterman function from the library
        return Fitness.smith_waterman(individual, word, match_score, mismatch_score, gap_penalty)

    def jaccard_similarity_fitness_c(self, individual, word):
        # Call the smith_waterman function from the library
        return Fitness.jaccard_similarity_fitness(individual, word)

    def modified_hamming_distance_fitness_p(self, individual, word):
        # Determine the lengths of the individual and the target word
        x = len(individual)
        y = len(word)
        # Calculate the absolute difference between the lengths
        diff_xy = abs(x-y)
        # Determine the minimum and maximum lengths of the two strings
        min_xy = min(x,y)
        max_xy = max(x,y)
        # Calculate the Hamming distance between the individual and the target word
        hd = sum([1 for i in range(min_xy) if individual[i] != word[i]])
        # Calculate the score as the Hamming distance plus the absolute length difference
        score = hd + diff_xy
        # Normalize the score to a value between 0 and 1
        score = score / max_xy
        # Return the score as the fitness of the individual
        return score

    def leveinstein_distance_fitness_p(self, individual, word):
        m = len(individual)
        n = len(word)
        
        # (n+1)
        distances = [[0] * (n + 1) for _ in range(m + 1)]

        # (m+1)
        for i in range(m + 1):
            distances[i][0] = i
        # (n+1)
        for j in range(n + 1):
            distances[0][j] = j     

        # (m+1) * (n+1) * (1+3) + (m+1)*1
        i_pred = 0
        for i in range(1, m + 1):
            j_pred = 0
            for j in range(1, n + 1):
                if individual[i_pred] == word[j_pred] :
                    distances[i][j] = distances[i_pred][j_pred]
                else:
                    distances[i][j] = min(distances[i][j_pred], distances[i_pred][j], distances[i_pred][j_pred]) + 1
                j_pred += 1
            i_pred += 1

        return distances[m][n] / max(m,n)

    def smith_waterman_p(self, individual, word, match_score=2, mismatch_score=-1, gap_penalty=-1):
        # Initialize the score matrix with zeros
        m = len(individual)
        n = len(word)
        score_matrix = [[0] * (n + 1) for _ in range(m + 1)]
        
        # Fill the score matrix
        i_pred = 0
        for i in range(1, m+1):
            j_pred = 0
            for j in range(1, n+1):
                match = match_score if individual[i_pred] == word[j_pred] else mismatch_score
                score_matrix[i][j] = max(
                    0, 
                    score_matrix[i_pred][j_pred] + match, 
                    score_matrix[i_pred][j] + gap_penalty, 
                    score_matrix[i][j_pred] + gap_penalty
                )
                j_pred += 1
            i_pred += 1
        
        # Find the highest score and its position in the score matrix
        max_score = 0
        for i in range(m+1):
            for j in range(n+1):
                if score_matrix[i][j] > max_score:
                    max_score = score_matrix[i][j]

        return 1 - (max_score / (max(m,n)*2))

    # Jaccard similarity 
    def jaccard_similarity_fitness_p(self, individual, word):
        len_diff = abs(len(individual)-len(word))
        # Convert individual into a set to remove duplicates
        individual_set = set(individual)                  
        # Convert word into a set to remove duplicates 
        word_set = set(word)                              
        # Calculate the size of the intersection of individual_set and word_set
        intersection = len(individual_set.intersection(word_set))
        # Calculate the size of the union of individual_set and word_set
        union = len(individual_set.union(word_set))                          
        # Calculate Jaccard similarity as the ratio of intersection to the sum of union and absolute difference in size between individual and word
        jaccard_similarity = intersection / (union + len_diff)
        return 1-jaccard_similarity

    # Normalized Longest Common Subsequence (NLCS)
    def nlcs_fitness_p(self, individual, word):
        # Initialize the match matrix with zeros
        individual_len = len(individual)
        word_len = len(word)            
        match_matrix = [[int(individual[i] == word[j]) for j in range(word_len)] for i in range(individual_len)]
        # Compute the length of the longest common subsequence using dynamic programming
        lcs_length = 0
        for i in range(1, individual_len):
            for j in range(1, word_len):
                if match_matrix[i-1][j-1]:
                    match_matrix[i][j] = match_matrix[i-1][j-1] + 1
                else:
                    match_matrix[i][j] = max(match_matrix[i-1][j-1], match_matrix[i][j])
                lcs_length = max(lcs_length, match_matrix[i][j])
        # Compute the NLCS score as the ratio of the length of the longest common subsequence to the length of the longer string
        nlcs_score = lcs_length / max(individual_len, word_len)
        # Return the complement of the NLCS score as the fitness of the individual
        return 1 - nlcs_score

    def cosine_similarity_fitness_p(self, individual, word):
        # Calculate the dot product of the vectors
        dot_product = 0
        for i in range(min(len(individual), len(word))):
            dot_product += ord(individual[i]) * ord(word[i])
        # Calculate the magnitudes of the vectors
        magnitude1 = math.sqrt(sum([ord(c)**2 for c in individual]))
        magnitude2 = math.sqrt(sum([ord(c)**2 for c in word]))
        # Calculate the cosine similarity
        if magnitude1 == 0 or magnitude2 == 0:
            return 1
        else:
            return 1 - (dot_product / (magnitude1 * magnitude2))
        

    def ngram_overlap_fitness_p(self, individual, word, n=2):
        min_len = min(len(individual), len(word))
        n = min_len if n > min_len else n
        # Split the strings into n-grams
        ngrams1 = [individual[i:i+n] for i in range(len(individual)-n+1)]
        ngrams2 = [word[i:i+n] for i in range(len(word)-n+1)]        
        # Compute the intersection of the two sets of n-grams
        intersection = set(ngrams1) & set(ngrams2)        
        # Compute the n-gram overlap score as the ratio of the size of the intersection to the size of the union
        overlap_score = len(intersection) / float(len(set(ngrams1) | set(ngrams2)))        
        return 1 - overlap_score   

    def manhattan_distance_fitness_p(self, individual, word):
        m = len(individual)
        n = len(word)    
        # Compute the Manhattan distance
        manhattan_distance = abs(sum(ord(individual[i]) - ord(word[j]) for i in range(m) for j in range(n)))
        # Compute the Manhattan distance fitness as the inverse of the distance
        manhattan_distance_fitness = manhattan_distance / (n*m * 96) # 96 is the maximum distance between two characters in the model        
        return manhattan_distance_fitness

    def pearson_correlation_fitness_p(self, individual, word):
        individual_len = len(individual)
        word_len = len(word)            
        vector_1 = [ord(x) for x in individual]
        vector_2 = [ord(x) for x in word]
        # Calculate the means of each vector
        mean1 = sum(vector_1) / individual_len
        mean2 = sum(vector_2) / word_len
        # Calculate the standard deviations of each vector
        std_dev1 = math.sqrt(sum((vector_1[x] - mean1) ** 2 for x in range(individual_len)) / (individual_len - 1))
        std_dev2 = math.sqrt(sum((vector_2[x] - mean2) ** 2 for x in range(word_len)) / (word_len - 1))
        # Calculate the covariance between the two vectors
        covariance = sum((vector_1[i] - mean1) * (vector_2[i] - mean2) for i in range(min(individual_len, word_len)))
        # Add 96 (maximal distance in the model) * 
        covariance += 96*abs(individual_len-word_len)
        # Calculate the Pearson correlation coefficient
        if std_dev1 * std_dev2 != 0:
            pearson_coeff = covariance / ((max(individual_len,word_len) - 1) * std_dev1 * std_dev2)
        else:
            pearson_coeff = 0
        # Return the Pearson correlation coefficient as the fitness of the individual
        return abs(1-pearson_coeff)
