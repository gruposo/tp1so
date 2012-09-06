#include "nodeADT.h"

struct nodeCDT {

	int command; //es la instruccion que debo ejecutar
	int param; //es el N
	nodeADT exe; //es la lista de instrucciones "Expr" dentro de un IF o un WHILE
	nodeADT returnTO; //para los END*, es un puntero para volver al IF o WHILE que les corresponde
	nodeADT jump; //para los IF y WHILE, les dice a que instruccion ir para saltar lo que esta en el medio
	nodeADT next; //para todos, es la instruccion siguiente
};

int getOperation(nodeADT node) {

	return node->command;
} //retorna que instruccion representa el nodo.

int getParam(nodeADT node) {

	return node->param;
} //retorna N.

nodeADT getNext(nodeADT node) {
	return node->next;
} //retorna un puntero al siguiente.

nodeADT getExe(nodeADT node) {
	return node->exe;
} //retorna lo que se ejecuta si se cumple la condicion

nodeADT getJump(nodeADT node) {
	return node->jump;
} //retorna la instruccion a la que debe saltar si no entra a un ciclo o a un IF

nodeADT getReturnTO(nodeADT node) {
	return node->returnTO;
} //retorna la instruccion a la que volver luego de un END*

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
