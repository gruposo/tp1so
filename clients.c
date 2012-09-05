#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define PATH_SIZE 1000

int main(void) {
	char client[PATH_SIZE];
	//char client2[PATH_SIZE];
	int pid, i,j;

	//sprintf(client1, "%s%d%s", "archivo", 2, ".txt");
	//sprintf(client2, "%s%d%s", "archivo", 3, ".txt");


	for (i = 0; i < 500; i++) {
		j = i % 5;
		sprintf(client, "%s%d", "archivo", j);  
		switch (pid = fork()) {

		case -1:
			printf("Server couldn't fork\n");
			exit(-1);
			break;
		case 0:
			execl("./clientGeneric", "clientGeneric", client, NULL );
			break;
		default:
			break;
		}
	}
	return 0;
}
