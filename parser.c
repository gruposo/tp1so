#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "parser.h"

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
	int c;
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
			toUpperString(string, index);
			com = getCommand(string, index);

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
					if (index != 0 && com == empty) {
						fatal();
					}
					fseek(file, -1, SEEK_CUR);
					return first;
				} else {
					fatal();
				}
			} else {
				vecBalance.numbers[vecBalance.pos] = 0;
			}
			if (index == 0) {
				if (com != cz) {
					fatal();
				} else {
					addParam(current, -1);
				}

			} else {
				if (com == endif) {
					if (vecIf.numbers[(vecIf.pos) - 1]
							!= toInt(string, index)) {
						fatal();
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
						fatal();
					}
					(vecWhile.pos) -= 1;

					nodeADT returnTO = (nodeADT) stack_top(&stack); //castear, maybe
					stack_pop(&stack, 0);
					addReturn(current, returnTO);
					addJump(returnTO, current);
					addParam(current, getParam(returnTO));

				} else if (com == cz) {
					fatal();
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
				fatal();
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
		fatal();
	}
	return first;
}

char *
resizeMemChar(int index, char * vec) {
	char * aux;
	if ((index % BLOQUE) == 0) {
		aux = realloc(vec, (index + BLOQUE) * sizeof(char));
		if (aux == NULL ) {
			printf(
					"There was a problem allocating memory. Try again\n");
			exit(-1);
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
					"There was a problem allocating memory. Try again\n");
			exit(-1);
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
			fatal();
		}
	}
}

int toInt(char*string, int index) {
	int N = 0;
	int mult = 1;
	int i;

	for (i = index - 1; i >= 0; i--) {
		if (!(string[i] >= '0') || !(string[i] <= '9')) {
			fatal();
		}
		N += ((string[i] - '0') * mult);
		mult *= 10;
	}

	return N;
}

Commands getCommand(char * string, int index) {
	Commands com = empty;

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
		fatal();
	}
	return com;
}

void fatal() {
	printf("Invalid parser. Please make another file\n");
	exit(-1);
}

void toUpperString(char * string, int index) {
	int i;
	for (i = 0; i < index; i++) {
		string[i] = toupper(string[i]);
	}
}
