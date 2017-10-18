#include "main.h"

int main () {
	// initialise
	char* characters = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ ";
	char* target = "Methinks it is like a weasel";

	int pop_size = 10;
	int numchars = (int)strlen(characters);
	int length = (int)strlen(target);

	char pop[pop_size][length+1];
	gen_population(numchars, characters, pop_size, length, pop);

	int pop_fitness[pop_size];
	for ( int i=0; i<pop_size; i++ ) {
		pop_fitness[i] = fitness(pop[i], target);
	}

	// print
	printf("  target: %s\n", target);
	for ( int i=0; i<pop_size; i++ ) {
		printf("member %i: %s -> %i\n", i, pop[i], pop_fitness[i]);
	}

	// return
	return 0;
}

/* create a population of random strings, all the length of target. */
void gen_population ( int numchars, char characters[numchars+1],
                      int size, int len_target, char pop[size][len_target+1] ) {
	for ( int j=0; j < size; j++ ) {
		char current[len_target+1];

		for ( int i=0; i < len_target; i++ ) {
			int idx = (int)floor(rand() % numchars);
			current[i] = characters[idx];
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