#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/fcntl.h>
#include <signal.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "ipc.h"

int sockfd;
int socket_flags;

int IPC_init(int pid, char * ipc_path){
    
    //  the local port on the client
    struct sockaddr_in client = {AF_INET, INADDR_ANY, INADDR_ANY};
    
    //  the remote address of the server
    struct sockaddr_in server = {AF_INET, 7000};
    
    //  convert and store the server's IP address
    server.sin_addr.s_addr = inet_addr("197.45.10.2");
    
    //  set up the transport end point
    if( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1){
        perror("socket call failed");
        exit(1);
    }
    
    /* Save the socket default flags */
	socket_flags = fcntl(sockfd,F_GETFL,0);
    
    //  bind the local address to the end point
    if ( bind(sockfd, (struct sockaddr *)&client, sizeof(struct sockaddr_in)) == -1){
        perror("bind call failed");
        exit(1);
    }
    
    return sockfd;
    
}

int IPC_connect(int fd, char * ipc_path){
    return fd;
}

void IPC_close(int fd, char * ipc_path){
    close(fd);
}

void IPC_send(message_t msg, int fd, int pid){
    
    //  the remote address of the server
    struct sockaddr_in server = {AF_INET, 7000};
    
    if( sendto(fd, &msg, sizeof (message_t), 0, (struct sockaddr *)&server, sizeof(struct sockaddr_in)) == -1){
        perror("client could not send a message");
//        return -1;
    }
    
}

message_t IPC_receive(int fd){
    message_t msg;
    if (recv(sockfd, &msg, sizeof (message_t), 0)== -1){
        perror("client could not receive a message");
//        return -1;
    }
    return msg;
}

int main(){
    return 1;
}
