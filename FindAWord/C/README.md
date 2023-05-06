# Find A Word C Code

## How to compile
```bash
git clone git@github.com:cgarcia31/genetic_algorithm_test.git
cd genetic_algorithm_test/FindAWord/C
```

### Release version

```bash
make
./find_a_word
```

The default compiler is clang.  
If you want to use GCC : 

```bash
make CC=gcc
``` 

### Debug version
```bash
make debug
gdb ./find_a_word
```

run to start the program in gdb

If you want to track memory leak

```bash
valgrind --track-origins=yes -s --leak-check=full --show-leak-kinds=all ./find_a_word
```


## How create the doc
```bash
make docs
```

## Change the main.c file
### The word to search
```c
const char * word = "Je vais bien, tu vas bien !";
```

### The population
```c
    int population_size = 1024;
    int min_individual_size = 2;
    int max_individual_size = 50;
    Population p = create_population(population_size, min_individual_size, max_individual_size);
```
The first value is the number of individuals  
The second is the minimum size of an individual  
And the last value is the maximum size of an individual  

min_individual_size must be less than or equal to the word length    
max_individual_size must be greater than or equal to the length of the word  

### The maximum of generations :  

```c
    int max_generations = 200000;
```
The program stops when the population generation reaches the value max_generations.

### The fitness functions

```c
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
```
The fitness function is chosen randomly with
```c
    rand_fitness = rand()%9;
```
You can set the value

### The selection functions

```c
    sf[0] = truncation_selection;
    sf[1] = roulette_wheel_selection;
    sf[2] = tournament_selection;
    sf[3] = rank_based_selection;
```
The selection function is chosen randomly with
```c
    rand_selection = rand()%4;
```
You can set the value

### The parent pairing functions
```c
    PairingFunction pf[3];
    pf[0] = random_pairing_parents;
    pf[1] = consecutive_pairing_parents;
    pf[2] = non_sequential_pairing_parents;
```
The pairing function is chosen randomly with
```c
    rand_pairing = rand()%3;
```
You can set the value

### The crossover functions :
```c
    cf[0] = uniform_crossover;
    cf[1] = multipoint_crossover;
    cf[2] = probalistic_crossover;
``` 
The crossover function is chosen randomly with
```c
    rand_crossover = rand()%3;
```
You can set the value

### The mutation functions
```c
    mf[0] = random_mutate;
    mf[1] = subsequence_inversion_mutate;
    mf[2] = swap_mutate;
    mf[3] = insertion_mutate;
    mf[4] = deletion_mutate;
```
The mutation function is chosen randomly with
```c
    rand_mutation = rand()%5;
```
You can set the value

Keep in mind that if you do not alternate the type of mutation, convergence may be difficult.



ToDo :
* Explain in detail all the functions of fitness, selection, matching, crossing and mutation.
* Manage the arguments of the main program to avoid code modification.
