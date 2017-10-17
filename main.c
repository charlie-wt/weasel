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

	// print
	printf("  target: %s\n", target);
	for ( int i=0; i<pop_size; i++ ) {
		printf("member %i: %s\n", i, pop[i]);
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