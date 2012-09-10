#ifndef IPC_H_
#define IPC_H_

#include "library.h"

typedef struct {
	int pid;
	char buffer[MAX_BUFFER_SIZE];
	int error;
} message_t;

void IPC_init(int pid, char * ipc_path);
int IPC_connect(int pid, char * ipc_path);
void IPC_close(int fd, char * ipc_path, int pid);
void IPC_send(message_t msg, int fd, int pid);
message_t IPC_receive(int fd, int pid);

#endif
