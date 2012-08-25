#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <semaphore.h>
#include <fcntl.h>
#define PATH_SIZE 32
#include "ipc.h"

int
main(int argc, char * argv[]) {
	message_t message, message2;
	int fd,fd2, pid, i;
	char private_fifo[PATH_SIZE], semaphore_path[PATH_SIZE];
	sem_t * semaphore;
	int * ans = malloc(1000 * sizeof(int));
	
	for(i=0; i<PATH_SIZE; i++){
		private_fifo[i] = '\0';
		semaphore_path[i] = '\0';
	}
	
	if(argc != 2) {
		printf("%s","Incorrect number of arguments\n");
		exit (-1);
	}
	
	pid = getpid();
	sprintf(semaphore_path, "%s%d", "/semaphore",pid);
	sprintf(private_fifo, "%s%d", "/tmp/fifo", pid);
	semaphore = sem_open(semaphore_path, O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH,0);
	
	message.pid = pid;
	strcpy(message.buffer,argv[1]);
	
	//envio al server el path para que abra el archivo y lo ejecute
	fd = IPC_connect(SERVER, "/tmp/fifo");
	IPC_send(message,fd, SERVER);
	
	sem_wait(semaphore);
	
	//recibo la respuesta del servidor
	fd2 = IPC_connect(SERVER, private_fifo);
	message2 = IPC_receive(fd2, pid);
	ans = (int *)deserialize_mem(message2.buffer);

//	for(i = 0; i < 1000; i++) {
//		printf("%d, ", ans[i]);
//	}
//	printf("\n");
	IPC_close(fd2,private_fifo, pid);
	
	sem_close(semaphore);
	sem_unlink(semaphore_path);
	
	return 0;
}
