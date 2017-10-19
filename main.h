#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

void gen_population ( int len_alpha, char alphabet[len_alpha+1],
                      int size, int len_target, char pop[size][len_target+1] );

int fitness ( char* individual, char* target );

void mutate ( unsigned int len, char original[len+1], char mutant[len+1],
               unsigned int len_alpha, char alphabet[len_alpha+1] );

int tournament ( unsigned int k, int pop_size, int len_target,
                 char pop[pop_size][len_target+1],
                 int pop_fitness[pop_size] );

int contains ( int size, int* arr, int val );

unsigned int hill_climber ( char* target, char* alphabet );

unsigned int ga_no_crossover ( int pop_size, char* target,
                               char* alphabet, int tournament_k );

void print_arr ( int size, int* arr );
