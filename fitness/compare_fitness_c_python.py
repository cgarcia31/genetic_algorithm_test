import random
import time
import Fitness

# Function to generate randomly a word
def random_word(len):
    word = []
    # Creating a list of ASCII characters from 32 to 127
    list_char = list(range(32,128))
    # Generating a random word of given length
    for _ in range(len):
        word.append(chr(random.choice(list_char)))
    return ''.join(word)

# Function to test a method in a language
def test_function(methods_dict, method, language, number_of_iterations, word1, word2):
    # Encoding the words if the language is C
    if language == "C":
        word1 = word1.encode('utf-8')
        word2 = word2.encode('utf-8')
    
    # Measuring the time taken by the method to execute
    t1 = time.monotonic_ns()
    for _ in range(number_of_iterations):
        score = methods_dict[method][language](word1, word2)
    t2 = time.monotonic_ns()
    duration = t2 - t1
    return score, int(duration/number_of_iterations)

# Function to run tests on all methods and languages for all word pairs
def run_tests(methods_dict, number_of_iterations, min_len_word, max_len_word, csv_file):
    # Creating and opening the csv file for writing the results
    with open(csv_file, 'w', newline='') as csvfile:
        # Writing the header row in the csv file
        header_csv = "num_test:len_word1:len_word2:method:language:score:duration(ns)\n"
        csvfile.write(header_csv)
        num = 0
        # Iterating over all word pairs of lengths between min_len_word and max_len_word
        for len_w1 in range(min_len_word,max_len_word+1):
            for len_w2 in range(min_len_word,max_len_word+1):
                # Generating random words of given lengths
                word1 = random_word(len_w1)
                word2 = random_word(len_w2)
                # Testing all methods and languages for the word pair
                for method in methods_dict.keys():
                    for language in methods_dict[method].keys():
                        # Testing a method in a language and getting the score and duration
                        score, duration = test_function(methods_dict, method, language, number_of_iterations, word1, word2)
                        # Writing the result in the csv file
                        line = "{}:{}:{}:{}:{}:{}:{}\n".format(num+1,len_w1,len_w2,method, language, round(score,4),duration)
                        csvfile.write(line)
                num += 1

def main():
    # Loading the fitness library
    Fitness.Fitness.load_fitness_lib("./fitness.so")
    fitn = Fitness.Fitness()

    # Creating a dictionary of all methods and languages to test
    methods_dict = {
                        "Hamming Distance": 
                        {
                            "C": fitn.modified_hamming_distance_fitness_c,
                            "Python": fitn.modified_hamming_distance_fitness_p
                        },
                        "Leveinstein Distance":
                        {
                            "C": fitn.leveinstein_distance_fitness_c,
                            "Python": fitn.leveinstein_distance_fitness_p
                        },
                        "Smith Waterman":
                        {
                            "C": fitn.smith_waterman_c,
                            "Python": fitn.smith_waterman_p
                        },
                        "Jaccard Similarity":
                        {
                            "C": fitn.jaccard_similarity_fitness_c,
                            "Python": fitn.jaccard_similarity_fitness_p
                        }
                    }
    # Running tests on all word pairs and writing the results to a csv file
    run_tests(methods_dict, 250, 1, 100, "examples/csv_files/compare_method_language.csv")

if __name__ == "__main__":
    main()
