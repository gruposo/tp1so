#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/fcntl.h>
#include <signal.h>
#include <unistd.h>
#include <netinet/in.h>

#include "ipc.h"

int IPC_init(int pid, char * ipc_path){
    
	int sockfd;

    //  creo el socket address local
    struct sockaddr_in socket_address = {AF_INET, 7000, INADDR_ANY};

    if(pid == SERVER){
    //  fijo la IP del servidor
    	socket_address.sin_addr.s_addr = inet_addr("197.45.10.2");
	}
    
    //  creo el socket
    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1){
        perror("socket call failed");
        exit(1);
    }
    
    //  bind del socket address con el socket creado
    if(bind(sockfd, (struct sockaddr *)&socket_address, sizeof(struct sockaddr_in))==1){
        close(sockfd);
        perror("bind call failed");
        exit(1);
    }
    
    return sockfd;
}

void IPC_send(message_t msg, int fd, int pid, sem_t * semaphore){
    
    // el socket address del destinatario
    struct sockaddr_in receiver; 
    int receiver_len = sizeof(struct sockaddr_in);

    // si quiero comunicarme con el server uso su IP fija
    if (pid == SERVER){
    	//  the local server port
    	receiver = {AF_INET, 7000, INADDR_ANY};
    	receiver.sin_addr.s_addr = inet_addr("197.45.10.2");
	} else {
		receiver = calloc(1,receiver_len);
    	receiver = *msg.sockaddr_p;
	}
    
    int length = sizeof(int) + MAX_BUFFER_SIZE * sizeof(char);
    char * serialized = calloc(1, length);
    
    serialized = serialize_msg(msg);
    
    //	manda el mensaje al destinatario receiver
    if( sendto(fd, serialized, length, 0, (struct sockaddr *)&receiver, receiver_len)== -1){
        perror("server could not send message");
//        return -1;
    }
    
}

message_t IPC_receive(int fd, int pid, sem_t * semaphore){

    message_t msg;
    
    //  el socket address en el cual se guarda el socket address del emisor
    struct sockaddr_in sender;
    unsigned int sender_len = sizeof(struct sockaddr_in);
    
    int length = sizeof(int) + MAX_BUFFER_SIZE * sizeof(char);
    char * serialized = calloc(1, length);
    
    //	recive el mensaje y guarda el emisor en sender
    if(recvfrom(fd, serialized, length, 0, (struct sockaddr *)&sender, &sender_len) == -1 ){
        perror("server could not receive message");
//        return -1;
    }

    //	adjunto al mensaje la direccion del socket address del emisor
    msg.sockaddr_p = &sender;
    
    return msg;
}

int IPC_connect(int pid, char * ipc_path){
    return fd;
}


void IPC_close(int fd, char * ipc_path, int pid){
    close(fd);
}