#include<fcntl.h>
#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<limits.h>
#include "fifo.h"
#define MSGSIZ 63

main() {
	int fd, c, length;
	char msgbuf[MSGSIZ + 1];
	FILE * file;
	char * private_fifo;
	char * file_path;

	if (mkfifo(fifo, 0666) == -1) {
		if (errno != EEXIST) {
			perror("Reciever: mkfifo");
		}
	}

	if ((fd = open(fifo, O_RDWR)) < 0) {
		perror("Fifo open fail");
	}

	for (;;) {

		if (read(fd, &length, sizeof(int)) < 0) {

			perror("No se pudo leer1");
		}

		printf("%d\n", length);

		file_path = calloc(length + 1, sizeof(char));

		if (read(fd, file_path, length) < 0) {

			perror("No se pudo leer1");
		}

		printf("%s\n", file_path);

//		if (read(fd, private_fifo, sizeof(char*)) < 0) {
//			perror("No se pudo leer2");
//		}

		file = fopen(file_path, "r");
		while ((c = fgetc(file)) != EOF) {
			printf("%c", c);
		}
		fclose(file);

	}
}

