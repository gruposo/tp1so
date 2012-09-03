#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define PATH_SIZE 1000

int main(void) {
	char client1[PATH_SIZE];
	char client2[PATH_SIZE];
	int pid, i;

	sprintf(client1, "%s%d%s", "archivo", 2, ".txt");
	sprintf(client2, "%s%d%s", "archivo", 3, ".txt");

	for (i = 0; i < 1000; i++) {
		switch (pid = fork()) {
		
		case -1:
			printf("Server couldn't fork\n");
			exit(-1);
			break;
		case 0:
			execl("./clientGeneric", "clientGeneric", client1, NULL );
			break;
		default:
			//execl("/home/juan/Descargas/tp1so/clientGeneric", "clientGeneric",client2,NULL);
			break;
		}
	}
	//si alguien esta leyendo y vienen y me escriben el que intenta escribir se bloquea, ahi tengo que desbloquearlo
	return 0;
}
