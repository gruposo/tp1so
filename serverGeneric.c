#include "serverGeneric.h"

int public_fd = 0;

int main(void) {
	int fd, pid;
	message_t message;
	static struct sigaction act;
	char * path = "/ipc";

	IPC_init(SERVER, path);
	fd = IPC_connect(SERVER, path);
	public_fd = fd;
	void catchSignal(int signal);
	act.sa_handler = catchSignal;
	sigfillset(&(act.sa_mask));
	sigaction(SIGABRT, &act, NULL );

	while (1) {
		message = IPC_receive(fd, SERVER);

		switch (pid = fork()) {
		case -1:
			printf("Server couldn't fork\n");
			exit(-1);
			break;
		case 0:
			programAttention(message.buffer, message.pid);
			exit(1);
		default:
			break;
		}
	}

	return 0;
}

void programAttention(char * filepath, int pid) {
	int fd, i;
	message_t message;
	nodeADT first;
	Block my_block;
	char * path = "/ipc";
	FILE * file;

	my_block.boolean = FALSE;
	my_block.current = 0;
	message.error = 0;

	for (i = 0; i < 1000; i++) {
		(my_block.memory)[i] = 0;
	}

	if ((file = fopen(filepath, "r")) != NULL ) {
		if (!feof(file)) {
			first = parse(file, FALSE);
		}

		fclose(file);
		execute(first, &my_block);

		memcpy(message.buffer, serialize_mem(my_block.memory), 4000);
	} else {
		message.error = ERROR;
	}

	fd = IPC_connect(pid, path);
	IPC_send(message, fd, pid);
}

void catchSignal(int signal) {
	char * path = "/ipc";
	printf("ABORT was catched. Closing all IPCS\n");
	IPC_close(public_fd, path, SERVER);
	exit(1);
}
