#ifndef SERIALIZER_H_
#define SERIALIZER_H_

#include "ipc.h"

char * serialize_msg(message_t * msg);
message_t * deserialize_msg(char * serialized_msg);
char * serialize_mem(int * mem);
int * deserialize_mem(char * serialized_mem);

#endif
