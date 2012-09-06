#ifndef SHMEM_H_
#define SHMEM_H_

void copyToMsg(int fd, message_t * message);
void copyFromMsg(int fd, message_t * message);

#endif
