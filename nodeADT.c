#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "nodeADT.h"

struct nodeCDT{

	int instr;
	int param;
	nodeADT cond;
	nodeADT next;
};

nodeADT newNode(int instr){

	nodeADT resp = malloc(sizeof(struct nodeCDT));

	if(resp == NULL){

		fprintf(stderr, "Memoria insuficiente\n");
		exit(1);
	}

	resp->instr = instr;

	return resp;
}

void addParam(nodeADT node, int param) {
	node->param = param;
}

void addCondList(nodeADT node, nodeADT condList){

	node->cond = condList;
}

void addNextList(nodeADT node, nodeADT nextList){

	node->next = nextList;
}

void print(nodeADT node) {
	printf("ins: %d   param: %d\n",node->instr,node->param);
}

nodeADT getNext(nodeADT node) {
	return node->next;
}

nodeADT getCond(nodeADT node) {
	return node->cond;
}
