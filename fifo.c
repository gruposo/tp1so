#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "ipc.h"

void
IPC_init(int pid, char * ipc_path) {
	char fifo_path[PATH_SIZE];
	
	sprintf(fifo_path, "%s%s%d", "/tmp",ipc_path, pid);
	
	if(mkfifo(fifo_path, 0666) == -1) {
		printf("Couldn't create fifo\n");
		exit(-1);
	}
}

int 
IPC_connect(int pid, char * ipc_path){
	int fd;
	char fifo_path[PATH_SIZE];
	
	sprintf(fifo_path, "%s%s%d", "/tmp",ipc_path, pid);
	
	if ((fd = open(fifo_path, O_RDWR)) < 0) {
		printf("%s","Couldn't connect to the fifo\n");
	}
	
	return fd;
}


void 
IPC_send(message_t msg, int fd, int pid) {
	int nwrite;
	
	if ((nwrite = write(fd, &msg, sizeof(msg))) == -1) {
		printf("%s","Message could not be send\n");
	}
}

message_t
IPC_receive(int fd, int pid) {
	message_t message;
	
	if (read(fd, &message,sizeof(message)) < 0) {
		printf("%s","Couldn't read from fifo\n");
	}
	
	return message;
}

void
IPC_close(int fd, char * ipc_path, int pid) {
	char fifo_path[PATH_SIZE];
	
	sprintf(fifo_path, "%s%s%d", "/tmp",ipc_path, pid);
	
	close(fd);
	unlink(fifo_path);
}
