#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "nodeADT.h"
#include "parser.h"

int main(void) {
	FILE * file;
	nodeADT first;

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
	return 0;
}
//distinguir entre recursivas y no recursivas, ver de acuerdo a eso si cortar el algoritmo por error o simplemente volver a la anterior.
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

			if(isFirst){
				first = newNode(com);
				current = first;
				isFirst = FALSE;
			}else{
				nodeADT next = newNode(com);
				addNextList(current, next);
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
				}else{
					addParam(current, -1);
				}
				
			} else {
				if (com == endif) {
					if (vecIf.numbers[(vecIf.pos) - 1] != toInt(string, index)) {
						fprintf(stderr, "Parser invalido\n");
						exit(1);
					}
					(vecIf.pos) -= 1;
				} else if (com == endwhile) {
					if (vecWhile.numbers[(vecWhile.pos) - 1] != toInt(string, index)) {
						fprintf(stderr, "Parser invalido\n");
						exit(1);
					}
					(vecWhile.pos) -= 1;
				} else if (com == cz) {
					fprintf(stderr, "Parser invalido\n");
					exit(1);
				} else {
					hasNumbers(string,index);
					
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
				
				addCondList(current,parse(file, TRUE));
				
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

void
printList(nodeADT node) {
	print(node);
	if(getCond(node) != NULL) {
		printList(getCond(node));
	}
	if(getNext(node) != NULL) {
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


