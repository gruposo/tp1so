#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "nodeADT.h"
#include "parser.h"
#include "Stack.h"

#define MEMSIZE 1000

int main(void) {
	FILE * file;
	nodeADT first;
	Block my_block;

	my_block.boolean = 0;

	my_block.current = 0;

	my_block.memory = calloc(MEMSIZE, sizeof(int));

	file = fopen("/home/facundo/tp1so/archivo.txt", "r");
	if (file == NULL) {
		printf("File couldn't be opened/n");
		exit(1);
	}

	if (!feof(file)) {
		first = parse(file, FALSE);
		printList(first);
	}

	fclose(file);

	execute(first, &my_block);

	return 0;
}

void execute(nodeADT node, Block * my_block) {

	int operation = getOperation(node);

	int N = (getParam(node) >= MEMSIZE) ? (MEMSIZE - 1) : getParam(node);

	switch (operation) {

	case (1):

		(my_block->memory)[my_block->current] += getParam(node);

		if (getNext(node) != null) {

			execute(getNext(node), my_block);
		}

		//hago el INC

		break;
	case (2):

		(my_block->memory)[my_block->current] -= getParam(node);

		if (getNext(node) != null) {

			execute(getNext(node), my_block);
		}

		//hago el DEC

		break;
	case (3):

		if ((my_block->current) + getParam(node) >= MEMSIZE) {

			my_block->current = MEMSIZE - 1;
		} else {

			(my_block->current) += getParam(node);
		}

		if (getNext(node) != null) {

			execute(getNext(node), my_block);
		}

		//hago el MR

		break;
	case (4):

		if ((my_block->current) - getParam(node) < 0) {

			my_block->current = 0;
		} else {

			(my_block->current) -= getParam(node);
		}

		if (getNext(node) != null) {

			execute(getNext(node), my_block);
		}

		//hago el ML

		break;
	case (5):

		if ((my_block->memory)[my_block->current]) {

			my_block->boolean = 1;
		} else {
			my_boolean = 0;
		}

		if (getNext(node) != null) {

			execute(getNext(node), my_block);
		}

		//hago el CZ

		break;
	case (6):

		if (my_block->boolean) {

			if (getExe(node) != null) {

				execute(getExe(node), my_block);
			}
		}

		if (getNext() != null) {

			nodeADT nodeENDIF = getJump(node);

			execute(getJump(node), my_block);


			//tal vez, si deberia salir del ciclo, estaria bueno seguir la recursividad con EL SIGUIENTE DEL ENDIF
			//ver que onda con el estado logico en el que estaba al entrar al if, para ver como se comportaria en el endif, y lo mismo para while.
		}

		//hago el IF

		break;
	case (7):

		//hago el ENDIF

		break;
	case (8):

		//hago el WHILE

		break;
	case (9):

		//hago el ENDWHILE

		break;

	default:

		break;
	}
}

nodeADT parse(FILE * file, int state) {
	nodeADT first;
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
			if (strcmp(string, "INC") == 0) {
				com = inc;
			} else if (strcmp(string, "DEC") == 0) {
				com = dec;
			} else if (strcmp(string, "MR") == 0) {
				com = mr;
			} else if (strcmp(string, "ML") == 0) {
				com = ml;
			} else if (strcmp(string, "CZ") == 0) {
				com = cz;
			} else if (strcmp(string, "IF") == 0) {
				com = ifa;
			} else if (strcmp(string, "ENDIF") == 0) {
				com = endif;
			} else if (strcmp(string, "WHILE") == 0) {
				com = whilea;
			} else if (strcmp(string, "ENDWHILE") == 0) {
				com = endwhile;
			} else {
				fprintf(stderr, "Parser invalido\n");
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
					fseek(file, -1, SEEK_CUR);
					return current;
				} else {
					fprintf(stderr, "Parser invalido\n");
					exit(1);
				}
			}
			if (index == 0) {
				if (com != cz) {
					fprintf(stderr, "Parser invalido\n");
					exit(1);
				} else {
					addParam(current, -1);
				}

			} else {
				if (com == endif) {
					if (vecIf.numbers[(vecIf.pos) - 1]
							!= toInt(string, index)) {
						fprintf(stderr, "Parser invalido\n");
						exit(1);
					}
					(vecIf.pos) -= 1;

					nodeADT returnTO = stack_top(&stack); //castear, maybe
					stack_pop(&stack, 1);
					addReturn(current, returnTO);
					addJump(returnTO, current);

				} else if (com == endwhile) {
					if (vecWhile.numbers[(vecWhile.pos) - 1]
							!= toInt(string, index)) {
						fprintf(stderr, "Parser invalido\n");
						exit(1);
					}
					(vecWhile.pos) -= 1;

					nodeADT returnTO = stack_top(&stack); //castear, maybe
					stack_pop(&stack, 1);
					addReturn(current, returnTO);
					addJump(returnTO, current);

				} else if (com == cz) {
					fprintf(stderr, "Parser invalido\n");
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
				fprintf(stderr, "Parser invalido\n");
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
		fprintf(stderr, "Parser invalido\n");
		exit(1);
	}

	return first;
}

void printList(nodeADT node) {
	print(node);
	if (getExe(node) != NULL) {
		printList(getExe(node));
	}
	if (getNext(node) != NULL) {
		printf("-------");
		printList(getNext(node));
	}

}

char *
resizeMemChar(int index, char * vec) {
	char * aux;
	if ((index % BLOQUE) == 0) {
		aux = realloc(vec, (index + BLOQUE) * sizeof(char));
		if (aux == NULL) {
			printf(
					"Hubo un problema al reservar memoria. Intente nuevamente\n");
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
		if (aux == NULL) {
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

