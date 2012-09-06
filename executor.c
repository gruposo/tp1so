#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "executor.h"
#include "nodeADT.h"

void execute(nodeADT node, Block * my_block) {

	if (node == NULL) {
		return;
	}

	int operation = getOperation(node);

//	int param = getParam(node);

//	int N = (param >= MEMSIZE) ? (MEMSIZE - 1) : param;

	void (*vecFun[])(nodeADT,
			Block *)= {_inc, _dec, _mr, _ml, _cz, _if, _endif, _while, _endwhile
	};

	(*vecFun[operation - 1])(node, my_block);
}

void _inc(nodeADT node, Block * my_block) {
	nodeADT next = getNext(node);

	((my_block->memory)[my_block->current]) += getParam(node);

	execute(next, my_block);

}

void _dec(nodeADT node, Block * my_block) {
	nodeADT next = getNext(node);

	((my_block->memory)[my_block->current]) -= getParam(node);

	execute(next, my_block);

}

void _mr(nodeADT node, Block * my_block) {
	nodeADT next = getNext(node);

	int param = getParam(node);

	if (((my_block->current) + param) >= MEMSIZE) {
		my_block->current = MEMSIZE - 1;
	} else {
		(my_block->current) += param;
	}
	execute(next, my_block);
}

void _ml(nodeADT node, Block * my_block) {
	nodeADT next = getNext(node);

	int param = getParam(node);

	if (((my_block->current) - param) < 0) {
		my_block->current = 0;
	} else {
		(my_block->current) -= param;
	}

	execute(next, my_block);
}

void _cz(nodeADT node, Block * my_block) {
	nodeADT next = getNext(node);

	my_block->boolean = ((my_block->memory)[my_block->current]) ? FALSE : TRUE;

	execute(next, my_block);
}

void _if(nodeADT node, Block * my_block) {

	execute(getExe(node), my_block);

	if ((my_block->boolean) == TRUE) {

		execute(getNext(node), my_block);
	} else {
		nodeADT nodeENDIF = (nodeADT) getJump(node);
		execute(getNext(nodeENDIF), my_block);
	}
}

void _endif(nodeADT node, Block * my_block) {
	nodeADT next = getNext(node);

	execute(next, my_block);
}

void _while(nodeADT node, Block * my_block) {
	execute(getExe(node), my_block);

	if ((my_block->boolean) == TRUE) {
		execute(getNext(node), my_block);
	} else {
		nodeADT nodeENDWHILE = (nodeADT) getJump(node);
		execute(getNext(nodeENDWHILE), my_block);
	}
}

void _endwhile(nodeADT node, Block * my_block) {
	execute((nodeADT) getReturnTO(node), my_block);
}
