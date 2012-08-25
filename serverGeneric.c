#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <semaphore.h>
#include <fcntl.h>
#include "parser.h"
#include "serializer.h"
#define PATH_SIZE 32
#include "ipc.h"
void programAttention(FILE * file, int pid);

int public_fd = 0;

int main(void) {
	int fd, pid, mqid;
	message_t message;
	FILE * file;
	static struct sigaction act;

	mqid = IPC_init(SERVER, "/tmp/fifo");
	fd = IPC_connect(SERVER, "/tmp/fifo");
	public_fd = fd;
	void catchSignal(int signal);
	act.sa_handler = catchSignal;
	sigfillset(&(act.sa_mask));
	sigaction(SIGABRT, &act, NULL );

	while (1) {
		message = IPC_receive(fd, SERVER);

		if ((file = fopen(message.buffer, "r")) != NULL ) {
			switch (pid = fork()) {
			case -1:
				printf("Server couldn't fork\n");
				exit(-1);
				break;
			case 0:
				programAttention(file, message.pid);
				break;
			default:
				break;
			}
		} else {
			printf("The file of the client %d couldn't be opened\n",
					message.pid);
		}
	}

	return 0;
}

void programAttention(FILE * file, int pid) {
	int c, fd, i, mqid;
	char private_fifo[PATH_SIZE], semaphore_path[PATH_SIZE];
	message_t message;
	sem_t * semaphore;
	char * serialized;

	nodeADT first;
	Block my_block;

	my_block.boolean = FALSE;
	my_block.current = 0;

	for (i = 0; i < 1000; i++) {

		(my_block.memory)[i] = 0;
	}
	(my_block.memory)[500] = 500;

	if (!feof(file)) {
		first = parse(file, FALSE);
	}

	fclose(file);
	printf("ANTES:/n");
	for(i = 0 ; i < 1000; i++) {
		printf("%d, ", (my_block.memory)[i]);
	}
	execute(first, &my_block);
	printf("DESPUES:/n");
	for(i = 0 ; i < 1000; i++) {
			printf("%d, ", (my_block.memory)[i]);
	}
	sprintf(private_fifo, "%s%d", "/tmp/fifo", pid);
	sprintf(semaphore_path, "%s%d", "/semaphore", pid);

	strcpy(message.buffer, serialize_mem(my_block.memory));

	semaphore = sem_open(semaphore_path, O_CREAT);

	mqid = IPC_init(SERVER, private_fifo);
	fd = IPC_connect(SERVER, private_fifo);

	sem_post(semaphore);
	IPC_send(message, fd, pid);
}

void catchSignal(int signal) {
	printf("ABORT was catched. Closing all IPCS\n");
	IPC_close(public_fd, "/tmp/fifo", SERVER);
	exit(1);
}
