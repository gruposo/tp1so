#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "nodeADT.h"

struct nodeCDT {

	int command;
	int param;
	nodeADT exe;
	nodeADT returnTO;
	nodeADT jump;
	nodeADT next;
};

int getOperation(nodeADT node) {

	return node->command;
}//retorna que operacion representa el nodo.

int getParam(nodeADT node) {

	return node->param;
}//retorna N.

nodeADT getNext(nodeADT node) {
	return node->next;
}//retorna un puntero al siguiente.

nodeADT getExe(nodeADT node) {
	return node->exe;
}//retorna lo que se ejecuta si se cumple la condicion

nodeADT newNode(int command) {

	nodeADT resp = malloc(sizeof(struct nodeCDT));

	if (resp == NULL) {

		fprintf(stderr, "Memoria insuficiente\n");
		exit(1);
	}

	resp->command = command;

	return resp;
}

void addParam(nodeADT node, int param) {
	node->param = param;
}

void addReturn(nodeADT node, nodeADT returnTO) {

	node->returnTO = returnTO;
}

void addExe(nodeADT node, nodeADT exeList) {

	node->exe = exeList;
}

void addNext(nodeADT node, nodeADT next) {

	node->next = next;
}

void addJump(nodeADT node, nodeADT jump) {

	node->jump = jump;
}

void print(nodeADT node) {
	printf("ins: %d   param: %d\n", node->command, node->param);
}
