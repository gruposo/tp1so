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

int sockfd;
int socket_flags;

int IPC_init(int pid, char * pathname){
    
    //  the local server port
    struct sockaddr_in server = {AF_INET, 7000, INADDR_ANY};
    
    //  set up the transport end point
    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1){
        perror("socket call failed");
        exit(1);
    }
    
    // Set SO_REUSEADDR on a socket to true (1).
	// This will help us if the server unexpectedly crashes and we need to
	// restart it. If we do not use this option, we must remove manually the
	// /tmp/socket_server file. This will prevent bind call failures
    
	int optval = 1;
    
	if (setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&optval,sizeof(int)) == -1) {
	    perror("setsockopt failed");
	    exit(1);
	}
    
    //  bind the local address to the end point
    if(bind(sockfd, (struct sockaddr *)&server, sizeof(struct sockaddr_in))==1){
        close(sockfd);
        perror("bind call failed");
        exit(1);
    }
    
    /* Save the socket default flags */
	socket_flags = fcntl(sockfd,F_GETFL,0);
    
    return sockfd;
}

int IPC_connect(int fd, char * pathname){
    return fd;
}


message_t IPC_receive(int fd){
    
    message_t msg;
    
    //  the structure to put in process2's address
    struct sockaddr_in client;
    unsigned int client_len = sizeof(struct sockaddr_in);
    
    int length = sizeof(int) + MAX_BUFFER_SIZE * sizeof(char);
    char * serialized = calloc(1, length);
    
    //  receives the message and stores the address of the client
    if(recvfrom(fd, serialized, length, 0, (struct sockaddr *)&client, &client_len) == -1 ){
        perror("server could not receive message");
//        return -1;
    }
    
    return msg;
    
}


void IPC_send(message_t msg, int fd, int pid){
    
    struct sockaddr_in client;  // A QUIEN LE MANDO??????????????????????????????????????????????
    int client_len = sizeof(struct sockaddr_in);
    
    int length = sizeof(int) + MAX_BUFFER_SIZE * sizeof(char);
    char * serialized = calloc(1, length);
    
    serialized = serialize_msg(msg);
    
    //        sends the message back to where it came from
    if( sendto(fd, serialized, length, 0, (struct sockaddr *)&client, client_len)== -1){
        perror("server could not send message");
//        return -1;
    }
    
}


void IPC_close(int fd, char * pathname){
    close(fd);
}


int main(){
    
    return 0;
    
}