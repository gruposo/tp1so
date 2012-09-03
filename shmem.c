#include "ipc.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/shm.h>

int
IPC_init(int pid, char * ipc_path) {
	return 0;
}

int
IPC_connect(int pid, char * ipc_path) {
	int shmid;

	if ((shmid = shmget(pid, sizeof(message_t), IPC_CREAT)) == -1) {
		printf("Couldn't allocate shared memory\n");
		exit(-1);
	}

printf("SHMID = %d\n", shmid);
	return shmid;
}

void
IPC_close(int fd, char * ipc_path, int pid) {

}

void
IPC_send(message_t msg, int fd, int pid, sem_t * semaphore) {
	message_t * memory;
	memory = (message_t *)shmat(fd, NULL,0);
	printf("ANTES DE ENVIAR\n");
	//memcpy(memory, &msg, sizeof(message_t));
	printf("DESPUES DE ENVIAR\n");
	sem_post(semaphore);
}

message_t
IPC_receive(int fd, int pid, sem_t * semaphore) {
	sem_wait(semaphore);
	void * memory;
	message_t message;
	
	memory = shmat(fd, NULL ,0);
	memcpy(&message, memory, sizeof(message_t));
	return message;
}
