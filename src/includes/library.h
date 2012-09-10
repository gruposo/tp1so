#ifndef LIBRARY_H_
#define LIBRARY_H_

#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/msg.h>
#include <errno.h>
#include <semaphore.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>

#define BLOQUE 10
#define FALSE 0
#define TRUE 1
#define MEMSIZE 1000
#define PATH_SIZE 40
#define SERVER 1025L
#define MAX_BUFFER_SIZE 4000
#define ERROR -1

void fatal(char * error);
void toUpperString(char * string, int index);
int toInt(char*string, int index);
void hasNumbers(char * vec, int dim);
int * resizeMemInt(int index, int * vec);
char * resizeMemChar(int index, char * vec);

#endif
