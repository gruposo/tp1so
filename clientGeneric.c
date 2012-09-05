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
	char fileName[PATH_SIZE];
	int * ans = malloc(VEC_SIZE * sizeof(int));
	FILE * file;
	char * path = "/ipc";
	
	if(argc != 2) {
		printf("Incorrect number of arguments for client %d\n", pid);
		exit (-1);
	}
	
	pid = getpid();
	
	message.pid = pid;
	strcpy(message.buffer,argv[1]);
	
	//envio al server el path para que abra el archivo y lo ejecute
	fd = IPC_connect(SERVER, path);
	mqid = IPC_init(pid, path);
	fd2 = IPC_connect(pid, path);
	IPC_send(message,fd, SERVER);
	
	//recibo la respuesta del servidor
	message2 = IPC_receive(fd2, pid);
	ans = (int *)deserialize_mem(message2.buffer);
	printResult(pid, argv[1], ans);
	
	IPC_close(fd2, path, pid);
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
