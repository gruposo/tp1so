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
	char sem_public_path[PATH_SIZE];
	static struct sigaction act;
	sem_t * sem_public;
	
	sprintf(sem_public_path, "%s%d", "/semaphore", (int)SERVER);
	sem_public = sem_open(sem_public_path, O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH,0);
	mqid = IPC_init(SERVER, "/tmp/fifo");
	fd = IPC_connect(SERVER, "/tmp/fifo");
	public_fd = fd;
	void catchSignal(int signal);
	act.sa_handler = catchSignal;
	sigfillset(&(act.sa_mask));
	sigaction(SIGABRT, &act, NULL );

	while (1) {
		message = IPC_receive(fd, SERVER, sem_public);
		printf("LLEGO\n");

		if ((file = fopen(message.buffer, "r")) != NULL ) {
			switch (pid = fork()) {
			case -1:
				printf("Server couldn't fork\n");
				exit(-1);
				break;
			case 0:
				programAttention(file, message.pid);
				exit(1);
			default:
				break;
			}
		} else {
			printf("The file of the client %d couldn't be opened\n", message.pid);
		}
	}

	return 0;
}

void programAttention(FILE * file, int pid) {
	int c, fd, i, mqid;
	char private_fifo[PATH_SIZE], sem_private_path[PATH_SIZE];
	message_t message;
	nodeADT first;
	Block my_block;
	sem_t * sem_private;
	
	my_block.boolean = FALSE;
	my_block.current = 0;
	
	for (i = 0; i < 1000; i++) {
		(my_block.memory)[i] = 0;
	}
	
	if (!feof(file)) {
		first = parse(file, FALSE);
	}
	
	fclose(file);
	execute(first, &my_block);
	
	sprintf(private_fifo, "%s%d", "/tmp/fifo", pid);
	sprintf(sem_private_path, "%s%d", "/semaphore", pid);
	
	sem_private = sem_open(sem_private_path, O_CREAT);
	
	memcpy(message.buffer, serialize_mem(my_block.memory), 4000);
	

	fd = IPC_connect(pid, private_fifo);
	IPC_send(message, fd, pid, sem_private);
}

void catchSignal(int signal) {
	printf("ABORT was catched. Closing all IPCS\n");
	IPC_close(public_fd, "/tmp/fifo", SERVER);
	exit(1);
}
