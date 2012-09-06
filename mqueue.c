#include "ipc.h"

int firstTime = 1;
int global_mqid = 0;

void
IPC_init(int pid, char * ipc_path) {
	return ;
}

int
IPC_connect(int pid, char * ipc_path) {
	int mqid;
	
	if(firstTime) {
		if((mqid = msgget(pid, 0664 | IPC_CREAT)) == -1) {
			printf("Couldn't create message queue\n");
			exit(-1);
		}
		global_mqid = mqid;
	}
	
	firstTime = 0;
	
	return global_mqid;
}


void
IPC_close(int fd, char * pathname, int pid) {
	
	if(pid == SERVER) {
		if((msgctl(fd, IPC_RMID, NULL)) == -1) {
			printf("Couldn't close the message queue\n");
			exit(-1);
		}
	}
}

void 
IPC_send(message_t msg, int fd, int pid) {
	struct to_send {
		long mtype;
		message_t message;
	};
	
	struct to_send send;
	
	send.mtype = pid;
	send.message = msg;
		
	if((msgsnd(fd,&send,sizeof(struct to_send) - sizeof(long), 0)) == -1) {
		if(errno == EAGAIN) {
			printf("Server couldn't send via message queue\n");
		}
	}
}

message_t
IPC_receive(int fd, int pid) {
	struct to_receive {
		long mtype;
		message_t message;
	};
	
	struct to_receive receive;
	
	if(msgrcv(fd, &receive, sizeof(struct to_receive) - sizeof(long), pid, 0) == -1) {
		printf("Server couldn't retrieve message from message queue\n");
	}
	
	return receive.message;
}

