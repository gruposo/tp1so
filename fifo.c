#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include "ipc.h"

int
IPC_init(int pid, char * pathname) {
	if(mkfifo(pathname, 0666) == -1) {
		printf("%s", "Couldn't create fifo\n");
	}
	
	return 0;
}

int
IPC_connect(int mqid, char * pathname) {
	int fd;
	
	if ((fd = open(pathname, O_RDWR)) < 0) {
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
	close(fd);
	unlink(ipc_path);
}
