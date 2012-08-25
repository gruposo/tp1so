#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ipc.h"
#include "serializer.h"

//int main(){
//
//    message_t test = {50, "hola"};
//
//    int length = sizeof(int) + MAX_BUFFER_SIZE * sizeof(char);
//    char * serial = calloc(1, length);
//
//    serial = serialize_msg(&test);
//
//    message_t * deserial = deserialize_msg(serial);
//    printf("%s\n",deserial->buffer);
//
//    return 1;
//}

//int main(){
//	int i;
//	int * test = malloc(10*sizeof(int));
//	for(i=0;i<10;i++){
//			test[i]=i;
//		}
//
//
//	int * t2 = malloc(10*sizeof(int));
//	char * str = malloc(10*sizeof(int));
//	str = (char *)serialize_mem(test);
//
//	t2 = (int *)deserialize_mem(str);
//	for(i=0;i<10;i++){
//		printf("%d\n",t2[i]);
//	}
//}

char * serialize_msg(message_t * msg){
    
    int length = sizeof(int) + MAX_BUFFER_SIZE * sizeof(char);
    char * serialized_msg = malloc(length);
    
    memcpy(serialized_msg, &(msg->pid), sizeof(int));
    
    int offset = sizeof(int);
    memcpy(serialized_msg+offset, msg->buffer, MAX_BUFFER_SIZE);
    
    return serialized_msg;
    
}

char * serialize_mem(int * mem){
    
    int length = sizeof(int) * 10;
    char * serialized_mem = calloc(length,sizeof(char));

    memcpy(serialized_mem, mem, length * sizeof(char));
    
    return serialized_mem;
    
}

message_t * deserialize_msg(char * serialized_msg){
    
    message_t * msg = calloc(1, sizeof(message_t));
    
    memcpy(&msg->pid, serialized_msg, sizeof(int));
    
    int offset = sizeof(int);
    memcpy(&msg->buffer, serialized_msg+offset, MAX_BUFFER_SIZE);
    
    return msg;
    
}

int * deserialize_mem(char * serialized_mem){
    
	int length = sizeof(int) * 10;
    int * mem = calloc(length, sizeof(char));
    
    memcpy(mem, serialized_mem, length);
    
    return mem;
    
}
