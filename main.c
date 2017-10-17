#include "main.h"

int main () {
	// our alphabet of characters to choose from.
	char characters[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ ";
	int numchars = (int)strlen(characters);

	// the phrase to get.
	char* target = "Methinks it is like a weasel";
	int length = (int)strlen(target);

//	// the string to evolve.
//	char current[length+1];
//	for ( int i=0; i < length; i++ ) {
//		int idx = (int)floor(rand() % numchars);
//		current[i] = characters[idx];
//	}
//	current[length] = '\0';
//
//	printf("   Target string: %s\n", target);
//	printf("Generated string: %s\n", current);

	printf("--- testing gen_pop ---\n");
	int pop_size = 10;
	char pop[pop_size][length+1];
	gen_population(numchars, characters, pop_size, length, pop);
	for ( int i=0; i<pop_size; i++ ) {
		printf("member %i: %s\n", i, pop[i]);
	}

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
//		pop[j] = current;
		strcpy(pop[j], current);
	}
}