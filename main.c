#include "main.h"

int main () {
	// initialise
	char* alphabet = "abcdefghijklmnopqrstuvwxyz ";
	char* target = "methinks it is like a weasel";

//	int pop_size = 10;
//	int len_alpha = (int)strlen(alphabet);
//	int length = (int)strlen(target);
//
//	char pop[pop_size][length+1];
//	gen_population(len_alpha, alphabet, pop_size, length, pop);
//
//	int pop_fitness[pop_size];
//	for ( int i=0; i<pop_size; i++ ) {
//		pop_fitness[i] = fitness(pop[i], target);
//	}
//
//	// print
//	printf("  target: %s\n", target);
//	for ( int i=0; i<pop_size; i++ ) {
//		printf("member %i: %s -> %i\n", i, pop[i], pop_fitness[i]);
//	}
//
//	// perform tournament selection
//	char* winner = pop[tournament(5, pop_size, length, pop, pop_fitness)];
//	printf("the winner was %s\n", winner);
//
//	// mutate the winner
//	char mutant[length+1];
//	mutate(length, winner, mutant, len_alpha, alphabet);
//	printf("now mutated to %s\n", mutant);
	int attempts = hill_climber(target, alphabet);
	printf("took %i attempts with the hill climber.\n", attempts);

	// return
	return 0;
}

/* create a population of random strings, all the length of target. */
void gen_population ( int len_alpha, char alphabet[len_alpha+1],
                      int size, int len_target, char pop[size][len_target+1] ) {
	for ( int j=0; j < size; j++ ) {
		char current[len_target+1];

		for ( int i=0; i < len_target; i++ ) {
			int idx = rand() % len_alpha;
			current[i] = alphabet[idx];
		}
		current[len_target] = '\0';

		strcpy(pop[j], current);
	}
}

/* fitness function -> how many positions have the right character? */
int fitness ( char* individual, char* target ) {
	int length = strlen(target);

	if ( strlen(individual) == length ) {
		int score = 0;
		for ( int i=0; i<length; i++ ) {
			if ( individual[i] == target[i] ) {
				score++;
			}
		}
		return score;
	}
	return -1;
}

/* change on average 1 character, and return the new string. */
void mutate ( unsigned int len, char original[len+1], char mutant[len+1],
               unsigned int len_alpha, char alphabet[len_alpha+1] ) {
	unsigned int mutations = 0;
	for ( int i=0; i<len; i++ ) {
		if ( !(rand() % len) ) {
			mutant[i] = alphabet[rand() % len_alpha];
			mutations++;
		} else {
			mutant[i] = original[i];
		}
	}
	mutant[len] = '\0';
}

/* tournament selection: pick k individuals, then return the one with the
 * highest fitness. */
int tournament ( unsigned int k, int pop_size, int len_target,
                 char pop[pop_size][len_target+1],
                 int pop_fitness[pop_size] ) {
	// get tournament members
	int idxs[k];
	idxs[0] = rand() % pop_size;
	for ( int i=1; i<k; i++ ) {
		unsigned int idx = rand() % pop_size;
		while ( contains(i, idxs, idx) ) {  // only check first i+1 members.
			idx = rand() % pop_size;
		}
		idxs[i] = idx;
	}

	// have tournament
	int best_fitness = 0;
	int best_individual = idxs[0];
	for ( int i=0; i<k; i++ ) {
		if ( pop_fitness[idxs[i]] > best_fitness ) {
			best_fitness = pop_fitness[idxs[i]];
			best_individual = idxs[i];
		}
	}

	printf("in the tournament of members ");
	print_arr(k, idxs);
	printf(", the winner was %i (fitness %i)\n", best_individual, best_fitness);
	return best_individual;
}

int contains ( int size, int* arr, int val ) {
	for ( int i=0; i<size; i++ ) {
		if ( arr[i] == val ) {
			return 1;
		}
	}
	return 0;
}

void print_arr ( int size, int* arr ) {
	printf("%i", arr[0]);
	for ( int i=1; i<size-1; i++ ) {
		printf(", %i", arr[i]);
	}
	if ( size > 1 ) { printf(" & %i", arr[size-1]); }
}

unsigned int hill_climber ( char* target, char* alphabet ) {
	/* get info. */
	int len_alpha  = (int)strlen(alphabet);
	int len_target = (int)strlen(target);

	/* initialise climber. */
	char climber[len_target+1];
	for ( int i=0; i < len_target; i++ ) {
		int idx = rand() % len_alpha;
		climber[i] = alphabet[idx];
	}
	climber[len_target] = '\0';
	int climber_fitness = fitness(climber, target);

	printf("start: %s, fitness %i\n", climber, climber_fitness);

	unsigned int attempts = 0;
	/* climb. */
	while ( strcmp(climber, target) ) {
		attempts++;

		/* mutate the climber. */
		char mutant[len_target+1];
		mutate(len_target, climber, mutant, len_alpha, alphabet);

		/* reinsert the winner, and recalculate fitness. */
		int mutant_fitness = fitness(mutant, target);
		if ( mutant_fitness > climber_fitness ) {
			printf("%i: new best: %s, fitness %i\n", attempts, mutant, mutant_fitness);
			strcpy(climber, mutant);
			climber_fitness = mutant_fitness;
		}
	}
	printf("== Solution reached. ==\n");

	return attempts;
}

/* genetic algorithm, no crossover. TODO - replacing members of population. */
unsigned int ga_no_crossover ( int pop_size, char* target,
                               char* alphabet, int tournament_k ) {
	/* get info. */
	int len_alpha  = (int)strlen(alphabet);
	int len_target = (int)strlen(target);

	/* generate population. */
	char pop[pop_size][len_target+1];
	gen_population(len_alpha, alphabet, pop_size, len_target, pop);

	/* get fitness of population. */
	int pop_fitness[pop_size];
	for ( int i=0; i<pop_size; i++ ) {
		pop_fitness[i] = fitness(pop[i], target);
	}

	/* print. */
	printf("  target: %s\n", target);
	for ( int i=0; i<pop_size; i++ ) {
		printf("member %i: %s -> %i\n", i, pop[i], pop_fitness[i]);
	}

	int best = tournament(tournament_k, pop_size, len_target, pop, pop_fitness);
	char* winner = pop[best];
	unsigned int attempts = 0;

	/* climb. */
	while ( strcmp(winner, target) ) {
		attempts++;

		/* perform tournament selection. */
		best = tournament(tournament_k, pop_size, len_target, pop, pop_fitness);
		winner = pop[best];

		/* mutate the winner. */
		char mutant[len_target+1];
		mutate(len_target, winner, mutant, len_alpha, alphabet);

		/* reinsert the winner, and recalculate fitness. */
		// TODO
//		strcpy(pop[best], winner);
//		pop_fitness[best] = fitness(winner, target);
	}

	/* return number of attempts. */
	return attempts;
}

void test_rng ( int chance, int repeats ) {
	int hits = 0;
	int tries = 0;
	while ( hits < repeats ) {
		tries++;
		if ( !(rand() % chance) ) { hits++; }
	}
	printf("average: %f tries.\n", (tries / (float)repeats));
}