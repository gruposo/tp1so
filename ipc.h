#ifndef IPC_H_
#define IPC_H_

#define MEM_SIZE 5000
#define SERVER 1L

#define MAX_BUFFER_SIZE 5000

typedef struct {
	int pid;
	char buffer[MAX_BUFFER_SIZE];
	int contenttypesize;
	int contentsize;
} message_t;


//char file_path[BUFF_SIZE];
//int memory[MEM_SIZE];

int IPC_init(int pid, char * ipc_path);
//devuelve fd

int IPC_connect(int pid, char * ipc_path);
//devuelve fd

void IPC_close(int fd, char * ipc_path, int pid);

void IPC_send(message_t msg, int fd, int pid);

message_t IPC_receive(int fd, int pid);

#endif
