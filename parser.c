#include <stdio.h>
#include <stdlib.h>
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
		checkCommand(file);
	}
	
	fclose(file);
	return 0;
}

void
checkCommand(FILE * file) {
	tNumbers vec;
	vec.numbers = malloc((sizeof(int)) * BLOQUE);
	vec.pos = 0;
	
	
}