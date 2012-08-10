#define BLOQUE 10

typedef struct {
	int * numbers;
	int pos;
} tNumbers;

typedef enum {empty, inc, dec, mr, ml, cz, ifa, endif, whilea, endwhile} Commands;
void parse(FILE * file);
char * resizeMemChar(int index, char * vec);
int * resizeMemInt(int index, int * vec);
// void checkCommand(char * string)
