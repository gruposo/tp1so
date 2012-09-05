#ifndef PARSER_H_
#define PARSER_H_

#include "nodeADT.h"
#include "Stack.h"

#define BLOQUE 10
#define FALSE 0
#define TRUE 1
#define MEMSIZE 1000

typedef struct {
	int * numbers;
	int pos;
} tNumbers;

typedef enum {
	empty = 0, inc, dec, mr, ml, cz, ifa, endif, whilea, endwhile
} Commands;

nodeADT parse(FILE * file, int state);
char * resizeMemChar(int index, char * vec);
int * resizeMemInt(int index, int * vec);
void hasNumbers(char * vec, int dim);
int toInt(char*string, int index);
Commands getCommand(char * string, int index);
void fatal();
void toUpperString(char * string, int index);

#endif
