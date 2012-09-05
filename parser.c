#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "parser.h"

#define MEMSIZE 1000

void execute(nodeADT node, Block * my_block) {

	if(node == NULL) {
		return ;
	}
	
	int operation = getOperation(node);

	int param = getParam(node);

	int N = (param >= MEMSIZE) ? (MEMSIZE - 1) : param;

	void (*vecFun[])(nodeADT,
			Block *)= {_inc, _dec, _mr, _ml, _cz, _if, _endif, _while, _endwhile
			};

	(*vecFun[operation - 1])(node, my_block);
}

void _inc(nodeADT node, Block * my_block) {

//	printf("INC -> N = %d\n", getParam(node));

	nodeADT next = getNext(node);

	((my_block->memory)[my_block->current]) += getParam(node);

	if (next != NULL ) {
		execute(next, my_block);
	}
}

void _dec(nodeADT node, Block * my_block) {

//	printf("DEC -> N = %d\n", getParam(node));

	nodeADT next = getNext(node);

	((my_block->memory)[my_block->current]) -= getParam(node);

	if (next != NULL ) {
		execute(next, my_block);
	}
}

void _mr(nodeADT node, Block * my_block) {

//	printf("MR -> N = %d\n", getParam(node));

	nodeADT next = getNext(node);

	int param = getParam(node);

	if (((my_block->current) + param) >= MEMSIZE) {
		my_block->current = MEMSIZE - 1;
	} else {
		(my_block->current) += param;
	}
	if (next != NULL ) {
		execute(next, my_block);
	}
}

void _ml(nodeADT node, Block * my_block) {

//	printf("ML -> N = %d\n", getParam(node));

	nodeADT next = getNext(node);

	int param = getParam(node);

	if (((my_block->current) - param) < 0) {
		my_block->current = 0;
	} else {
		(my_block->current) -= param;
	}
	if (next != NULL ) {
		execute(next, my_block);
	}
}

void _cz(nodeADT node, Block * my_block) {

//	printf("CZ -> N = %d\n", getParam(node));

	nodeADT next = getNext(node);

	my_block->boolean = ((my_block->memory)[my_block->current]) ? FALSE : TRUE;

	if (next != NULL ) {
		execute(next, my_block);
	}
}

void _if(nodeADT node, Block * my_block) {

//	printf("IF -> N = %d\n", getParam(node));

	execute(getExe(node), my_block);

	if ((my_block->boolean) == TRUE) {

		execute(getNext(node), my_block);
	} else {
		nodeADT nodeENDIF = (nodeADT) getJump(node);

		if (getNext(nodeENDIF) != NULL ) {

			execute(getNext(nodeENDIF), my_block);
		}
	}
}

void _endif(nodeADT node, Block * my_block) {

//	printf("ENDIF -> N = %d\n", getParam(node));

	nodeADT next = getNext(node);

	if (next != NULL ) {

		execute(next, my_block);
	}
}

void _while(nodeADT node, Block * my_block) {

//	printf("WHILE -> N = %d\n", getParam(node));

	execute(getExe(node), my_block);
	if ((my_block->boolean) == TRUE) {

		execute(getNext(node), my_block);
	} else {

		nodeADT nodeENDWHILE = (nodeADT) getJump(node);

		if (getNext(nodeENDWHILE) != NULL ) {

			execute(getNext(nodeENDWHILE), my_block);
		}
	}
}

void _endwhile(nodeADT node, Block * my_block) {

//	printf("ENDWHILE -> N = %d\n", getParam(node));

	execute((nodeADT) getReturnTO(node), my_block);
}

nodeADT parse(FILE * file, int state) {
	nodeADT first = NULL;
	nodeADT current;
	tNumbers vecWhile;
	tNumbers vecIf;
	tNumbers vecBalance;
	vecWhile.numbers = malloc((sizeof(int)) * BLOQUE);
	vecWhile.pos = 0;
	vecIf.numbers = malloc(sizeof(int) * BLOQUE);
	vecIf.pos = 0;
	vecBalance.numbers = calloc(sizeof(int), BLOQUE);
	vecBalance.pos = 0;
	char * string = malloc(sizeof(char) * BLOQUE);
	int index = 0;
	int c, i;
	Commands com = empty;
	int isFirst = TRUE;

	Stack stack;

	stack_init(&stack);

	while ((c = fgetc(file)) != EOF) {
		switch (c) {
		case '(':
			vecBalance.numbers = resizeMemInt(vecBalance.pos,
					vecBalance.numbers);
			(vecBalance.numbers)[vecBalance.pos] = c;
			for (i = 0; i < index; i++) {
				string[i] = toupper(string[i]);
			}
			if (strncmp(string, "INC", index) == 0) {
				com = inc;
			} else if (strncmp(string, "DEC", index) == 0) {
				com = dec;
			} else if (strncmp(string, "MR", index) == 0) {
				com = mr;
			} else if (strncmp(string, "ML", index) == 0) {
				com = ml;
			} else if (strncmp(string, "CZ", index) == 0) {
				com = cz;
			} else if (strncmp(string, "IF", index) == 0) {
				com = ifa;
			} else if (strncmp(string, "ENDIF", index) == 0) {
				com = endif;
			} else if (strncmp(string, "WHILE", index) == 0) {
				com = whilea;
			} else if (strncmp(string, "ENDWHILE", index) == 0) {
				com = endwhile;
			} else {
				fprintf(stderr, "Parser invalido1\n");
				exit(1);
			}

			if (isFirst) {
				first = newNode(com);
				current = first;
				isFirst = FALSE;
			} else {
				nodeADT next = newNode(com);
				addNext(current, next);
				current = next;
			}

			index = 0;
			break;
		case ')':
			if (vecBalance.numbers[vecBalance.pos] != '(') {
				if (state) {
					if(index != 0 && com == empty) {
						printf("Invalid parser\n");
						exit(-1);
					}
					fseek(file, -1, SEEK_CUR);
					return first;
				} else {
					fprintf(stderr, "Parser invalido2\n");
					exit(1);
				}
			} else {
				vecBalance.numbers[vecBalance.pos] = 0;
			}
			if (index == 0) {
				if (com != cz) {
					fprintf(stderr, "Parser invalido3\n");
					exit(1);
				} else {
					addParam(current, -1);
				}

			} else {
				if (com == endif) {
					if (vecIf.numbers[(vecIf.pos) - 1]
							!= toInt(string, index)) {
						fprintf(stderr, "Parser invalido4\n");
						exit(1);
					}
					(vecIf.pos) -= 1;

					nodeADT returnTO = (nodeADT) stack_top(&stack);
					stack_pop(&stack, 0);
					addReturn(current, returnTO); //le digo al ENDIF cual es su IF correspondiente
					addJump(returnTO, current); //le digo al IF cual es su ENDIF correspondiente
					addParam(current, getParam(returnTO));

				} else if (com == endwhile) {
					if (vecWhile.numbers[(vecWhile.pos) - 1]
							!= toInt(string, index)) {
						fprintf(stderr, "Parser invalido5\n");
						exit(1);
					}
					(vecWhile.pos) -= 1;

					nodeADT returnTO = (nodeADT) stack_top(&stack); //castear, maybe
					stack_pop(&stack, 0);
					addReturn(current, returnTO);
					addJump(returnTO, current);
					addParam(current, getParam(returnTO));

				} else if (com == cz) {
					fprintf(stderr, "Parser invalido6\n");
					exit(1);
				} else {
					hasNumbers(string, index);

					addParam(current, toInt(string, index));
				}
				vecBalance.numbers[vecBalance.pos] = 0;
				index = 0;
			}
			break;
		case ',':
			if (index != 0 && (com == ifa || com == whilea)) {

				hasNumbers(string, index);
				addParam(current, toInt(string, index));

				if (com == ifa) {
					vecIf.numbers = resizeMemInt(vecIf.pos, vecIf.numbers);
					vecIf.numbers[(vecIf.pos)++] = toInt(string, index);

				} else {
					vecWhile.numbers = resizeMemInt(vecWhile.pos,
							vecWhile.numbers);
					vecWhile.numbers[(vecWhile.pos)++] = toInt(string, index);
				}

				stack_push(&stack, current);

				addExe(current, parse(file, TRUE));

			} else {
				fprintf(stderr, "Parser invalido7\n");
				exit(1);
			}
			break;
		case ' ':
		case '\n':
		case '\t':
			break;
		default:
			string = resizeMemChar(index, string);
			string[index++] = c;
			break;
		}
	}
	if (vecBalance.numbers[vecBalance.pos] != 0) {
		fprintf(stderr, "Parser invalido8\n");
		exit(1);
	}
	return first;
}

char *
resizeMemChar(int index, char * vec) {
	char * aux;
	if ((index % BLOQUE) == 0) {
		aux = realloc(vec, (index + BLOQUE) * sizeof(char));
		if (aux == NULL ) {
			printf("Hubo un problema al reservar memoria. Intente nuevamente\n");
			exit(1);
		} else {
			vec = aux;
		}
	}
	return vec;
}

int *
resizeMemInt(int index, int * vec) {
	int * aux;
	if ((index % BLOQUE) == 0) {
		aux = realloc(vec, (index + BLOQUE) * sizeof(int));
		if (aux == NULL ) {
			printf(
					"Hubo un problema al reservar memoria. Intente nuevamente\n");
			exit(1);
		} else {
			vec = aux;
		}
	}
	return vec;
}

void hasNumbers(char * vec, int dim) {
	int i;
	for (i = 0; i < dim; i++) {
		if (!isdigit(vec[i])) {
			fprintf(stderr, "Parser invalido\n");
			exit(1);
		}
	}
}

int toInt(char*string, int index) {
	int N = 0;
	int mult = 1;
	int i;
	
	for (i = index - 1; i >= 0; i--) {
		if (!(string[i] >= '0') || !(string[i] <= '9')) {
			fprintf(stderr, "Parser invalido\n");
			exit(1);
		}
		N += ((string[i] - '0') * mult);
		mult *= 10;
	}
	
	return N;
}
