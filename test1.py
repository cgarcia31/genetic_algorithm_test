import random

# Function to choose a random character
def choose_a_char():
    return chr(random.randint(32,128))

# Function to generate an individual with n characters
def generate_individual(n):
    return [choose_a_char() for _ in range(n)]

# Function to generate a population of individuals with individual_size characters
def generate_population(population_size, individual_size):
    return [generate_individual(individual_size) for _ in range(population_size)]

# Function to calculate the fitness of an individual compared to a word or sentence
def fitness(individual, word):
    return sum(1 for x in range(len(word)) if individual[x] != word[x])

# Function for selection: selecting a portion of the population based on fitness score
def selection(population, rate, word):
    selected_size = int(len(population) * rate)
    fitness_scores = [(idx, fitness(individual,word)) for idx, individual in enumerate(population)]
    sorted_scores = sorted(fitness_scores, key=lambda x: x[1], reverse=False)
    selected_indices = [idx for idx, _ in sorted_scores[:selected_size]]
    return [population[i] for i in selected_indices]

# Function for crossover: combining genetic material from two individuals to create a child
def crossover(x,y):
    individual_size = len(x)
    return x[:individual_size//2] + y[individual_size//2:] if random.random() >0.5 else y[:individual_size//2] + x[individual_size//2:]

# Function to generate children through crossover and mutation
def generate_children(population, population_size, mutation_rate):
    number_of_children = min(len(population)-1, population_size-len(population))
    new_pop = list(population)
    liste_indices = list(range(len(population)))
    position_scores = [(len(population) - i) for i in range(len(population))]
    total_score = sum(position_scores)
    probabilities = [score / total_score for score in position_scores]

    for _ in range(number_of_children):
        couple = random.choices(liste_indices, weights=probabilities, k=2)
        child = crossover(population[couple[0]], population[couple[1]])
        new_pop.append(mutate(child, len(child), mutation_rate))
    return new_pop

# Function for mutation: introducing changes in an individual's genetic material
def mutate(individual, size, mutation_rate):
    number_of_mutations  = int(size*mutation_rate)
    gene_to_modify = random.sample(range(size), number_of_mutations)
    for x in gene_to_modify:
        individual[x] = choose_a_char()
    return individual    

# Function to fill up the population to the desired size
def fill_pop(population, pop_size, individual_size):
    for _ in range(pop_size-len(population)):
        population.append(generate_individual(individual_size))
    return population

# Parameters for the genetic algorithm
population_size = 16
selection_rate = 0.8
mutation_rate = 0.02
word_or_sentence = "Hello how are you? I am doing well! My program is working perfectly, and I can continue for a long time."
individual_size = len(word_or_sentence)

# Generating the initial population
pop = generate_population(population_size, individual_size)

for i in range(100000):
    # Select the fittest individuals from the population
    pop = selection(pop, selection_rate, word_or_sentence)
    # Calculate the fitness of the best solution in the current population
    best_soluce = fitness(pop[0], word_or_sentence)
    # Prepare the string to display information about the best solution
    str_to_display = "The best soluce : " + "".join(pop[0]) + " - Soit : {:.2f}%".format((individual_size - best_soluce)/individual_size*100)
    # Print the information about the best solution
    print(str_to_display)
    # Check if the best solution has a fitness score of 0 (i.e., it matches the target word exactly)
    if best_soluce == 0:
        # Print the generation number where the solution was found
        print(f" \nsolution found in {i} generations ")
        # Exit the loop as the solution has been found
        break
    # Generate children from the selected individuals in the population
    pop = generate_children(pop, population_size, mutation_rate)
    # Fill the remaining population slots with new individuals
    pop = fill_pop(pop, population_size, individual_size)
