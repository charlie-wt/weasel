#include "main.h"

int main () {
	/* initialise. */
	char* alphabet = "abcdefghijklmnopqrstuvwxyz ";
	char* target = "methinks it is like a weasel";

	/* hill climber. */
	printf("=== HILL CLIMBER ===\n");
	int hill_attempts = hill_climber(target, alphabet, 0);
	printf("took %i attempts with the hill climber.\n", hill_attempts);

	/* non-crossover ga */
	printf("=== GA, NO CROSSOVER ===\n");
	int nga_attempts = ga_no_crossover(10, target, alphabet, 2, 0);
	printf("took %i attempts with the non-crossover genetic algorithm.\n", nga_attempts);

	/* ga */
	printf("=== GA, WITH CROSSOVER ===\n");
	int ga_attempts = ga(10, target, alphabet, 2, 0);
	printf("took %i attempts with the genetic algorithm.\n", ga_attempts);

	/* return. */
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
	for ( int i=0; i<len; i++ ) {
		if ( !(rand() % len) ) {
			mutant[i] = alphabet[rand() % len_alpha];
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
                 int pop_fitness[pop_size], int winners, int return_winner ) {
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

	// when selecting a member to remove at each step, use inverse-tournament.
	int better (int a, int b) { return a > b; }
	int worse (int a, int b) { return a < b; }
	int (*compete)(int a, int b);
	if (return_winner) {
		compete = better;
	} else {
		compete = worse;
	}

	// have tournament
	// TODO - could modify tournament to return a list of the n best
	// (involves returning an array/pointer even if 1 winner), or just make a
	// new 2-tournament function.
	int best_fitness = pop_fitness[idxs[0]];
	int best_individual = idxs[0];
	for ( int i=1; i<k; i++ ) {
		if ( compete(pop_fitness[idxs[i]], best_fitness) ) {
			best_fitness = pop_fitness[idxs[i]];
			best_individual = idxs[i];
		}
	}

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

unsigned int hill_climber ( char* target, char* alphabet, int prnt ) {
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

	if (prnt) { printf("start: %s, fitness %i\n", climber, climber_fitness); }

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
			if (prnt) { printf("%i: new best: %s, fitness %i\n", attempts, mutant, mutant_fitness); }
			strcpy(climber, mutant);
			climber_fitness = mutant_fitness;
		}
	}

	/* return number of attempts. */
	if (prnt) { printf("== Solution reached @ %i ==\n", attempts); }
	return attempts;
}

/* genetic algorithm, no crossover. */
unsigned int ga_no_crossover ( int pop_size, char* target, char* alphabet,
                               int tournament_k, int prnt ) {
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

	int best = tournament(tournament_k, pop_size, len_target, pop, pop_fitness, 1, 1);
	char* winner = pop[best];
	unsigned int attempts = 0;
	int best_fitness_so_far = pop_fitness[best];

	/* evolve. */
	while ( strcmp(winner, target) ) {
		attempts++;

		/* perform tournament selection. */
		best = tournament(tournament_k, pop_size, len_target, pop, pop_fitness, 1, 1);
		winner = pop[best];

		/* mutate the winner. */
		char mutant[len_target+1];
		mutate(len_target, winner, mutant, len_alpha, alphabet);

		/* reinsert the winner, and recalculate fitness. */
		int to_delete = tournament(tournament_k, pop_size, len_target, pop, pop_fitness, 1, 0);
		strcpy(pop[to_delete], mutant);
		pop_fitness[to_delete] = fitness(mutant, target);

		if ( prnt && pop_fitness[to_delete] > best_fitness_so_far ) {
			printf("%i: new best: %s, fitness %i\n", attempts, mutant, pop_fitness[to_delete]);
			best_fitness_so_far = pop_fitness[to_delete];
		}
	}

	/* return number of attempts. */
	if (prnt) { printf("== Solution reached @ %i ==\n", attempts); }
	return attempts;
}

/* genetic algorithm, with crossover. TODO - crossover. */
unsigned int ga ( int pop_size, char* target, char* alphabet,
                  int tournament_k, int prnt ) {
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

	int best = tournament(tournament_k, pop_size, len_target, pop, pop_fitness, 1, 1);
	char* winner = pop[best];
	unsigned int attempts = 0;
	int best_fitness_so_far = pop_fitness[best];

	/* evolve. */
	while ( strcmp(winner, target) ) {
		attempts++;

		/* perform tournament selection. */
		best = tournament(tournament_k, pop_size, len_target, pop, pop_fitness, 1, 1);
		winner = pop[best];

		/* mutate the winner. */
		char mutant[len_target+1];
		mutate(len_target, winner, mutant, len_alpha, alphabet);

		/* reinsert the winner, and recalculate fitness. */
		int to_delete = tournament(tournament_k, pop_size, len_target, pop, pop_fitness, 1, 0);
		strcpy(pop[to_delete], mutant);
		pop_fitness[to_delete] = fitness(mutant, target);

		if ( prnt && pop_fitness[to_delete] > best_fitness_so_far ) {
			printf("%i: new best: %s, fitness %i\n", attempts, mutant, pop_fitness[to_delete]);
			best_fitness_so_far = pop_fitness[to_delete];
		}
	}

	/* return number of attempts. */
	if (prnt) { printf("== Solution reached @ %i ==\n", attempts); }
	return attempts;
}