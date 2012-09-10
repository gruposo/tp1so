#include "shmem.h"

static sem_t * public_sem_wr = NULL;
static sem_t * public_sem_rd = NULL;

void
IPC_init(int pid, char * ipc_path) {
	if(pid == SERVER) {
	 	char public_path_wr[PATH_SIZE];
		char public_path_rd[PATH_SIZE];
		sprintf(public_path_wr, "%s%s", ipc_path, "wr");
		sprintf(public_path_rd, "%s%s", ipc_path, "rd");
		public_sem_wr = sem_open(public_path_wr, O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH, 1);
		public_sem_rd = sem_open(public_path_rd, O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH, 0);
	} else {
		char private_path[PATH_SIZE];
		sprintf(private_path, "%s%d", ipc_path, pid);
		sem_open(private_path, O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH, 0);
	}
}

int
IPC_connect(int pid, char * ipc_path) {
	int shmid;
	
	if ((shmid = shmget(pid, sizeof(message_t), 0666 | IPC_CREAT)) == -1) {
		printf("Couldn't allocate shared memory\n");
		exit(-1);
	}
	if(pid == SERVER) {
		char public_path_wr[PATH_SIZE];
		char public_path_rd[PATH_SIZE];
		sprintf(public_path_wr, "%s%s", ipc_path, "wr");
		sprintf(public_path_rd, "%s%s", ipc_path, "rd");
		public_sem_wr = sem_open(public_path_wr, O_CREAT);
		public_sem_rd = sem_open(public_path_rd, O_CREAT);
	}
	return shmid;
}

void
IPC_close(int fd, char * ipc_path, int pid) {
	if(pid == SERVER) {
		char public_path_wr[PATH_SIZE];
		char public_path_rd[PATH_SIZE];
		sprintf(public_path_wr, "%s%s", ipc_path, "wr");
		sprintf(public_path_rd, "%s%s", ipc_path, "rd");
		sem_close(public_sem_wr);
		sem_unlink(public_path_wr);
		sem_close(public_sem_rd);
		sem_unlink(public_path_rd);
	} else {
		char private_path[PATH_SIZE];
		sprintf(private_path, "%s%d", "/ipc", pid);
		sem_t * private_sem = sem_open(private_path, O_CREAT);
		sem_close(private_sem);
		sem_unlink(private_path);
	}
	close(fd);
	shmctl(fd, IPC_RMID, NULL);
}

void
IPC_send(message_t msg, int fd, int pid) {
	if(pid == SERVER) {
		sem_wait(public_sem_wr);
		copyFromMsg(fd, &msg);
		sem_post(public_sem_rd);
	} else {
		char private_path[PATH_SIZE];
		sprintf(private_path, "%s%d", "/ipc", pid);
		sem_t * private_sem = sem_open(private_path, O_CREAT);
		copyFromMsg(fd, &msg);
		sem_post(private_sem);
	}
}

message_t
IPC_receive(int fd, int pid) {
	message_t message;
	
	if(pid == SERVER) {
		sem_wait(public_sem_rd);
		copyToMsg(fd, &message);
		sem_post(public_sem_wr);
	} else {
		char private_path[PATH_SIZE];
		sprintf(private_path, "%s%d", "/ipc", pid);
		sem_t * private_sem = sem_open(private_path, O_CREAT);
		sem_wait(private_sem);
		copyToMsg(fd, &message);
	}
	return message;
}

void
copyToMsg(int fd, message_t * message) {
	void * memory;
	
	memory = shmat(fd, NULL, 0);
	memcpy(message, memory, sizeof(message_t));
	shmdt(memory);
}

void
copyFromMsg(int fd, message_t * message) {
	void * memory;
	
	memory = shmat(fd, NULL, 0);
	memcpy(memory, message, sizeof(message_t));
	shmdt(memory);
}
