#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <semaphore.h>
#include <fcntl.h>
#include "ipc.h"
#include "clientGeneric.h"

int
main(int argc, char * argv[]) {
	message_t message, message2;
	int fd,fd2,i, pid, mqid;
	char private_fifo[PATH_SIZE], semaphore_path[PATH_SIZE], fileName[PATH_SIZE];
	int * ans = malloc(VEC_SIZE * sizeof(int));
	FILE * file;
	sem_t * semaphore;
	
	for(i = 0; i < PATH_SIZE; i++){
		private_fifo[i] = '\0';
		semaphore_path[i] = '\0';
	}
	
	if(argc != 2) {
		printf("Incorrect number of arguments for client %d\n", pid);
		exit (-1);
	}
	
	pid = getpid();
	sprintf(semaphore_path, "%s%d", "/semaphore",pid);
	sprintf(private_fifo, "%s%d", "/tmp/fifo", pid);
	
	message.pid = pid;
	strcpy(message.buffer,argv[1]);
	
	//envio al server el path para que abra el archivo y lo ejecute
	fd = IPC_connect(SERVER, "/tmp/fifo");
	mqid = IPC_init(SERVER, private_fifo);
	fd2 = IPC_connect(SERVER, private_fifo);
	IPC_send(message,fd, SERVER);

	//recibo la respuesta del servidor

	message2 = IPC_receive(fd2, pid);
	ans = (int *)deserialize_mem(message2.buffer);
	printResult(pid, argv[1], ans);
	
	IPC_close(fd2,private_fifo, pid);
	
	return 0;
}

void
printResult(int pid, char * path, int * ans) {
	FILE * file;
	char fileName[PATH_SIZE];
	int i;
	
	sprintf(fileName, "%s%d", "client", pid);
	file = fopen(fileName, "w");
	if(file == NULL) {
		printf("File from client %d for answer couldn't be created\n", pid);
		return;
	}
	
	fprintf(file, "File executed: %s\n", path);
	for(i = 0; i < VEC_SIZE; i++) {
		  fprintf(file, "%d, ", ans[i]);
	}
	fprintf(file, "\n");
	fclose(file);
}
