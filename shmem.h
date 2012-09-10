#ifndef SHMEM_H_
#define SHMEM_H_

#include "ipc.h"

void copyToMsg(int fd, message_t * message);
void copyFromMsg(int fd, message_t * message);

#endif
