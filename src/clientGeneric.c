#include "includes/clientGeneric.h"

int main(int argc, char * argv[]) {
	message_t message, message2;
	int fd, fd2, pid;
	int * ans = malloc(MEMSIZE * sizeof(int));
	char * path = "/ipc";

	pid = getpid();

	if (argc != 2) {
		printf("Client: %d. ", pid);
		fatal("Incorrect number of arguments\n");
	}

	message.pid = pid;
	strcpy(message.buffer, argv[1]);

	//envio al server el path para que abra el archivo y lo ejecute
	fd = IPC_connect(SERVER, path);
	IPC_init(pid, path);
	fd2 = IPC_connect(pid, path);
	IPC_send(message, fd, SERVER);

	//recibo la respuesta del servidor
	message2 = IPC_receive(fd2, pid);
	if (message2.error != ERROR) {
		ans = (int *) deserialize_mem(message2.buffer);
		printResult(pid, argv[1], ans);
	} else {
		printf("The file of the client %d couldn't be opened\n", pid);
	}

	IPC_close(fd2, path, pid);
	free(ans);
	return 0;
}

void printResult(int pid, char * path, int * ans) {
	FILE * file;
	char fileName[PATH_SIZE];
	int i;

	sprintf(fileName, "../exit/%s%d", "client", pid);
	file = fopen(fileName, "w");
	if (file == NULL ) {
		printf("File from client %d for answer couldn't be created\n", pid);
		return;
	}

	fprintf(file, "File executed: %s\n", path);
	for (i = 0; i < MEMSIZE; i++) {
		fprintf(file, "%d, ", ans[i]);
	}
	fprintf(file, "\n");
	fclose(file);
}
