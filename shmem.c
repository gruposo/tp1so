#include "ipc.h"
#include <stdlib.h>
#include <sys/shm.h>

int IPC_init(int pid, char * ipc_path, sem_t semaphore) {
	return 0;
}

int IPC_connect(int pid, char * ipc_path, void * mem) {
	int shmid;

	if ((shmid = shmget(pid, sizeof(message_t), IPC_CREAT)) == -1) {
		printf("Couldn't allocate shared memory\n");
		exit(-1);
	}
	if ((mem = shmat(shmid, NULL, NULL )) == ((void *) -1)) {
		printf("Process %d couldn't attach to the shared memory\n", pid);
	}
	return shmid;
}

void IPC_close(int fd, char * ipc_path, int pid) {

}

void IPC_send(message_t msg, int fd, int pid) {

}

message_t IPC_receive(int fd, int pid) {

}
