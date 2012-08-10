#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "parser.h"

int 
main(void) {
	FILE * file;
	
	file = fopen("archivo.txt","r");
	if(file == NULL) {
		printf("File couldn't be opened/n");
		exit(1);
	}
	
	if(!feof(file)) {
		parse(file);
	}
	
	fclose(file);
	return 0;
}

void
parse(FILE * file) {
	tNumbers vecWhile;
	tNumbers vecIf;
	tNumbers vecBalance;
	vecWhile.numbers = malloc((sizeof(int)) * BLOQUE);
	vecWhile.pos = 0;
	vecIf.numbers = malloc(sizeof(int) * BLOQUE);
	vecIf.pos = 0;
	vecBalance.numbers = calloc(sizeof(int),BLOQUE);
	vecBalance.pos = 0;
	char * string = malloc(sizeof(char) * BLOQUE);
	char * aux;
	int index = 0;
	int c, i;
	Commands com = empty;
	
	while((c = fgetc(file)) != EOF) {
		switch(c) {
			case '(': 
				vecBalance.numbers = resizeMemInt(vecBalance.pos, vecBalance.numbers);
				(vecBalance.numbers)[vecBalance.pos] = c;
				//checkCommand(string, index);
				for(i = 0; i < index; i++) {
					toupper(string[i]);
				}	
				if(strcmp(string, "INC") == 0) {
					com = inc;
				} else if(strcmp(string, "DEC") == 0) {
					com = dec;
				} else if(strcmp(string, "MR") == 0) {
					com = mr;
				} else if(strcmp(string, "ML") == 0) {
					com = ml;
				} else if(strcmp(string, "CZ") == 0) {
					com = cz;
				} else if(strcmp(string, "IF") == 0) {
					com = ifa;
				} else if(strcmp(string, "ENDIF") == 0) {
					com = endif;
				} else if(strcmp(string, "WHILE") == 0) {
					com = whilea;
				} else if(strcmp(string, "ENDWHILE") == 0) {
					com = endwhile;
				} else {
					fprintf(stderr, "Parser invalido\n");
					exit(1);
				}
				index = 0;
				break;
			case ')': 
				if(vecBalance.numbers[vecBalance.pos] != '(') {
					exit(1);
				}
				if(index == 0) {
					if(com != cz) {
						fprintf(stderr, "Parser invalido\n");
						exit(1);
					}  
				} else {
					if(com == inc) {
					  
					} else if(com == dec) {
					  
					} else if(com == mr) {
					  
					} else if(com == ml) {
				  
				  
				  
				}
				} else if(com == inc && index != 0) {
				  
				  
				} else if(
				vecBalance.numbers[vecBalance.pos] = 0;
				index = 0;
				break;
			case ',':
				if(index != 0 && (com == ifa || com == whilea)) {
					if(com == ifa) {
						vecIf.numbers = resizeMemInt(vecIf.pos, vecIf.numbers);
						aux = malloc(sizeof(char) * (index - 1));
						hasNumbers(vecIf.numbers);
						vecIf.numbers[(vecIf.pos)++] = atoi(strncpy(aux ,string,index - 1));
						parse(file);
					} else {
						vecWhile.numbers = resizeMemInt(vecWhile.pos, vecWhile.numbers);
						aux = malloc(sizeof(char) * (index - 1));
						hasNumbers(vecWhile.numbers);
						vecWhile.numbers[(vecWhile.pos)++] = atoi(strncpy(aux,string,index - 1));
						parse(file);
					}
				} else {
					fprintf(stderr, "Parser invalido\n");
					exit(1);
				}
				break;
			case ' ': case '\n': case '\t': break;
			default: 
				string = resizeMemChar(index, string);
				string[index++] = c;
				break;
		}
	}
}

// void checkCommand(char * string, int dim, FILE * file) {
// 	int i;
// 	for(i = 0; i < dim; i++) {
// 		toUpper(string[i]);
// 	}
// 	
// 	if(strcmp(string, "INC") == 0) {
// 		
// 	} else if(strcmp(string, "DEC") == 0) {
// 	  
// 	} else if(strcmp(string, "MR") == 0) {
// 	  
// 	} else if(strcmp(string, "ML") == 0) {
// 	  
// 	} else if(strcmp(string, "CZ") == 0) {
// 	  
// 	} else if(strcmp(string, "IF") == 0) {
// 	  
// 	} else if(strcmp(string, "ENDIF") == 0) {
// 	  
// 	} else if(strcmp(string, "WHILE") == 0) {
// 	  
// 	} else if(strcmp(string, "ENDWHILE") == 0) {
// 	  
// 	} else {
// 		fprintf(stderr, "Parser invalido\n");
// 		exit(1);
// 	}
// }

char *
resizeMemChar(int index, char * vec) {
	char * aux;
	if((index % BLOQUE) == 0) {
		aux = realloc(vec,(index + BLOQUE) * sizeof(char));
		if(aux == NULL) {
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
	if((index % BLOQUE) == 0) {
		aux = realloc(vec,(index + BLOQUE) * sizeof(int));
		if(aux == NULL) {
			printf("Hubo un problema al reservar memoria. Intente nuevamente\n");
			exit(1);
			} else {
				vec = aux;
			}
	}
	return vec;
}

void
hasNumbers(int * vec, int dim) {
	int i;
	for(i = 0; i < dim; i++) {
		if(!isdigit(vec[i])) {
			fprintf(stderr, "Parser invalido\n");
			exit(1);
		}
	}
}
