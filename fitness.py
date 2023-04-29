def modified_hamming_distance_fitness(individual, word):
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

def leveinstein_distance_fitness(individual, word):
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

def smith_waterman(individual, word, match_score=2, mismatch_score=-1, gap_penalty=-1):
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
