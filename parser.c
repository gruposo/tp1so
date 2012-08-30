#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "parser.h"


#define MEMSIZE 1000

void execute(nodeADT node, Block * my_block) {

	int operation = getOperation(node);

	int param = getParam(node);

	int N = (getParam(node) >= MEMSIZE) ? (MEMSIZE - 1) : getParam(node);

	switch (operation) {

	case (1):

		printf("Operacion = INC / Parametro = %d\n", param);

		((my_block->memory)[my_block->current]) += getParam(node);
		if (getNext(node) != NULL) {
			execute(getNext(node), my_block);
		}
		//hago el INCthread vs for

		break;
	case (2):

		printf("Operacion = DEC / Parametro = %d\n", param);

		
		((my_block->memory)[my_block->current]) -= getParam(node);
		if (getNext(node) != NULL) {
			execute(getNext(node), my_block);
		}
		//hago el DEC

		break;
	case (3):

		printf("Operacion = MR / Parametro = %d\n", param);

		if (((my_block->current) + getParam(node)) >= MEMSIZE) {
			my_block->current = MEMSIZE - 1;
		} else {
			(my_block->current) += getParam(node);
		}
		if (getNext(node) != NULL) {
			execute(getNext(node), my_block);
		}
		//hago el MR

		break;
	case (4):

		printf("Operacion = ML / Parametro = %d\n", param);

		if (((my_block->current) - getParam(node)) < 0) {
			my_block->current = 0;
		} else {
			(my_block->current) -= getParam(node);
		}
		if (getNext(node) != NULL) {
			execute(getNext(node), my_block);
		}
		//hago el ML

		break;
	case (5):

		printf("Operacion = CZ / Parametro = %d\n", param);

		if (((my_block->memory)[my_block->current]) == 0) {
			my_block->boolean = TRUE;
		} else {
			my_block->boolean = FALSE;
		}
		if (getNext(node) != NULL) {
			execute(getNext(node), my_block);
		}
		//hago el CZ

		break;
	case (6):

		printf("Operacion = IF / Parametro = %d\n", param);

		execute(getExe(node), my_block); //SIEMPRE!!

		if ((my_block->boolean) == TRUE) {

			execute(getNext(node), my_block);
		} else {
			nodeADT nodeENDIF = (nodeADT) getJump(node);

			if (getNext(nodeENDIF) != NULL) {

				execute(getNext(nodeENDIF), my_block);
			}
		}
		//hago el IF

		break;
	case (7):

		printf("Operacion = ENDIF / Parametro = %d\n", param);

		if (getNext(node) != NULL) {

			execute(getNext(node), my_block);
		}
		//hago el ENDIF

		break;
	case (8):

		printf("Operacion = WHILE / Parametro = %d\n", param);

		execute(getExe(node), my_block);
		if ((my_block->boolean) == TRUE) {
			execute(getNext(node), my_block);
		} else {

			nodeADT nodeENDWHILE = (nodeADT) getJump(node);

			if (getNext(nodeENDWHILE) != NULL) {

				execute(getNext(nodeENDWHILE), my_block);
			}
		}
		//hago el WHILE

		break;
	case (9):

		printf("Operacion = ENDWHILE / Parametro = %d\n", param);

		execute((nodeADT) getReturnTO(node), my_block);
		//hago el ENDWHILE

		break;

	default:

		return;
		break;
	}

	return;
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

					nodeADT returnTO = (nodeADT) stack_top(&stack); //castear, maybe
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

//void printList(nodeADT node) {
//
//	print(node);
//
//	if (getExe(node) != NULL) {
//
//		printf("\t Expr - ABRE\n");
//
//		printList(getExe(node));
//
//		printf("\t Expr - CIERRA\n");
//	}
////
////	if ((nodeADT)getReturnTO(node) != NULL) {
////
////		printf("\t RETORNA A: ");
////
////		print((nodeADT)getReturnTO(node));
////	}
////
////	if ((nodeADT)getJump(node) != NULL) {
////
////		printf("\t SALTA A: ");
////
////		print((nodeADT)getJump(node));
////	}
//
//	if (getNext(node) != NULL) {
//		printf("->NEXT->");
//		printList(getNext(node));
//	}
//}

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

//void printMemory(Block * my_block) {
//
//	int i;
//	for (i = 0; i < 1000; i++) {
//
//		if ((my_block->memory)[i] != 0) {
//
//			printf("Memoria nro %d: %d\n", i, (my_block->memory)[i]);
//		}
//	}
//}

