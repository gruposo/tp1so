#ifndef PARSER_H_
#define PARSER_H_

#include "nodeADT.h"
#include "Stack.h"
#include "library.h"

typedef struct {
	int * numbers;
	int pos;
} tNumbers;

typedef enum {
	empty = 0, inc, dec, mr, ml, cz, ifa, endif, whilea, endwhile
} Commands;

nodeADT parse(FILE * file, int state);
Commands getCommand(char * string, int index);
void freeParser(char * string, int * vecWhile, int * vecIf, int * vecBalance, Stack * stack);

#endif
