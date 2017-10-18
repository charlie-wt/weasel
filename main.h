#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

void gen_population ( int numchars, char characters[numchars+1],
                      int size, int len_target, char pop[size][len_target+1] );

int fitness ( char* individual, char* target );
