#include "library.h"

void fatal(char * error) {
	printf("%s", error);
	exit(-1);
}

void toUpperString(char * string, int index) {
	int i;
	for (i = 0; i < index; i++) {
		string[i] = toupper(string[i]);
	}
}

int toInt(char*string, int index) {
	int N = 0;
	int mult = 1;
	int i;

	for (i = index - 1; i >= 0; i--) {
		if (!(string[i] >= '0') || !(string[i] <= '9')) {
			fatal("Invalid parser\n");
		}
		N += ((string[i] - '0') * mult);
		mult *= 10;
	}

	return N;
}

void hasNumbers(char * vec, int dim) {
	int i;
	for (i = 0; i < dim; i++) {
		if (!isdigit(vec[i])) {
			fatal("Invalid parser\n");
		}
	}
}

int *
resizeMemInt(int index, int * vec) {
	int * aux;
	if ((index % BLOQUE) == 0) {
		aux = realloc(vec, (index + BLOQUE) * sizeof(int));
		if (aux == NULL ) {
			fatal("There was a problem allocating memory. Try again\n");
		} else {
			vec = aux;
		}
	}
	return vec;
}

char *
resizeMemChar(int index, char * vec) {
	char * aux;
	if ((index % BLOQUE) == 0) {
		aux = realloc(vec, (index + BLOQUE) * sizeof(char));
		if (aux == NULL ) {
			fatal("There was a problem allocating memory. Try again\n");
		} else {
			vec = aux;
		}
	}
	return vec;
}
