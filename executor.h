#ifndef EXECUTOR_H_
#define EXECUTOR_H_

#include "nodeADT.h"
#include "library.h"

typedef struct {
	int param;
	int boolean;
	int current;
	int memory[MEMSIZE];
} Block;

void execute(nodeADT node, Block * my_block);
void _inc(nodeADT node, Block * my_block);
void _dec(nodeADT node, Block * my_block);
void _mr(nodeADT node, Block * my_block);
void _ml(nodeADT node, Block * my_block);
void _cz(nodeADT node, Block * my_block);
void _if(nodeADT node, Block * my_block);
void _endif(nodeADT node, Block * my_block);
void _while(nodeADT node, Block * my_block);
void _endwhile(nodeADT node, Block * my_block);

#endif
