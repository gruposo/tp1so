#include <unistd.h>
#include "includes/library.h"

int main(int argc, char ** argv) {
	char client[PATH_SIZE];
	int pid, i, j, qty, size;

	if (argc != 2) {
		fatal("Invalid parameters\n");
	}

	size = strlen(argv[1]);
	hasNumbers(argv[1], size);
	qty = toInt(argv[1], size);

	for (i = 0; i < qty; i++) {
		j = i % 3;
		sprintf(client, "%s%d%s", "../doc/archivo", j, ".txt");
		switch (pid = fork()) {

		case -1:
			fatal("Server couldn't fork\n");
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
