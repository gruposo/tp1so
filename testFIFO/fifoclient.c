#include<fcntl.h>
#include<stdio.h>
#include <stdlib.h>
#include<errno.h>
#include <limits.h>
#include <string.h>
#include "fifo.h"
#define MSGSIZ 63

main(int argc, char ** argv) {
	int fd, nwrite, j, pid;
	char * private_fifo;
	char * file_path;
	char * char_pid;
	int length;

	if (argc != 2) {
		perror("Invalid parameters/n");
		exit(1);
	}

	if ((fd = open(fifo, O_WRONLY | O_NONBLOCK)) < 0) {
		perror("Fifo open failed");
	}

	pid = getpid();

	char_pid = calloc(countDigits(pid) + 1, sizeof(char));

	file_path = calloc(strlen(argv[1]) + 1, sizeof(char));
	private_fifo = calloc(strlen("fifo") + countDigits(pid) + 1, sizeof(char));

	sprintf(char_pid, "%d", pid);

	sprintf(file_path, "%s", argv[1]);

	sprintf(private_fifo, "%s%s", "fifo", char_pid);

	for (j = 1; j < argc; j++) {
		if (strlen(argv[j]) > MSGSIZ) {
			printf("MUY largo el mensaje");
			continue;
		}

		length = strlen(file_path);

		printf("Length cliente: %d\n", length);

		if ((nwrite = write(fd, &length, sizeof(int))) == -1) {
			perror("No se pudo enviar el texto");
		}

		if ((nwrite = write(fd, file_path, strlen(file_path))) == -1) {
			perror("No se pudo enviar el texto");
		}

//		if ((nwrite = write(fd, private_fifo, sizeof(char*))) == -1) {
//			perror("No se pudo enviar el texto");
//		}
	}

	exit(0);
}

int countDigits(int number) {
	int count = 0;

	while (number != 0) {
		number /= 10;
		count++;
	}

	return count;
}
