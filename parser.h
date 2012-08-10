#define BLOQUE 10

typedef struct {
	int * numbers;
	int pos;
} tNumbers;

typedef enum {empty = 0, inc, dec, mr, ml, cz, ifa, endif, whilea, endwhile} Commands;
void parse(FILE * file);
char * resizeMemChar(int index, char * vec);
int * resizeMemInt(int index, int * vec);
void hasNumbers(char * vec, int dim);
// void checkCommand(char * string)
