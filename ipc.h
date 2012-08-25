#define MAX_BUFFER_SIZE 5000

typedef struct {
	int pid;
	char buffer[MAX_BUFFER_SIZE];
	int contenttypesize;
	int contentsize;
} message_t;

int IPC_init(int pid, char * ipc_path);
//devuelve fd

int IPC_connect(int fd, char * ipc_path);
//devuelve fd

void IPC_close(int fd, char * ipc_path);

void IPC_send(message_t msg, int fd, int pid);

message_t IPC_receive(int fd);