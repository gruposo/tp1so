#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/fcntl.h>
#include <signal.h>
#include <unistd.h>
#include <sys/un.h>
#include <netinet/in.h>
#include "ipc.h"

void
IPC_init(int pid, char * ipc_path) {
	return;
}

int
IPC_connect(int pid, char * ipc_path) {
	int sockfd;
	char to_path[PATH_SIZE];
	
	sprintf(to_path, "%s%s%d", "/tmp",ipc_path, pid);
	struct sockaddr_un socket_address;
	socket_address.sun_family = AF_UNIX;
	strcpy(socket_address.sun_path,to_path);
	
	if ((sockfd = socket(AF_UNIX, SOCK_DGRAM, 0)) == -1) {
		perror("socket call failed");
		exit(1);
	}
	
	if ((bind(sockfd, (struct sockaddr *) &socket_address, sizeof(struct sockaddr_un))) == 1) {
		close(sockfd);
		perror("bind call failed");
		exit(1);
	}

	return sockfd;
}

void 
IPC_send(message_t msg, int fd, int pid) {
	struct sockaddr_un receiver;
	char to_path[PATH_SIZE];
	
	sprintf(to_path, "%s%d", "/tmp/ipc", pid);
	receiver.sun_family = AF_UNIX;
	strcpy(receiver.sun_path,to_path);
	sendto(fd, &msg, sizeof(message_t), 0, (struct sockaddr *) &receiver, sizeof(receiver));
}

message_t 
IPC_receive(int fd, int pid) {
	message_t message;
	
	if(recv(fd, &message, sizeof(message_t), 0) == -1) {
		perror("Couldn't receive message\n");
		exit(-1);
	}
	
	return message;
}

void
IPC_close(int fd, char * ipc_path, int pid) {
	char path[PATH_SIZE];
	sprintf(path, "%s%s%d", "/tmp",ipc_path, pid);
	unlink(path);
	close(fd);
}
