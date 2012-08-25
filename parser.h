#ifndef PARSER_H_
#define PARSER_H_

#include "nodeADT.h"
#include "Stack.h"

#define BLOQUE 10
#define FALSE 0
#define TRUE 1

typedef struct {
	int param;
	int boolean;
	int current;
	int memory[1000];
} Block;

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
void printList(nodeADT node);
void execute(nodeADT node, Block * my_block);
void printMemory(Block * my_block);

#endif
