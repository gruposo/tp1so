#define BLOQUE 10
#define FALSE 0
#define TRUE !FALSE

typedef struct {
	int * numbers;
	int pos;
} tNumbers;

typedef enum {empty = 0, inc, dec, mr, ml, cz, ifa, endif, whilea, endwhile} Commands;
void parse(FILE * file,int state);
char * resizeMemChar(int index, char * vec);
int * resizeMemInt(int index, int * vec);
void hasNumbers(char * vec, int dim);
int toInt(char*string, int index);
// void checkCommand(char * string)
