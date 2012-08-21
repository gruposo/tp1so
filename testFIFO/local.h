#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<linux/limits.h>
#include<errno.h>

#define B_SIZE (PIPE_BUF/2)

const char * PUBLIC = "/tmp/fifos";

struct message{

	char fifo_name[B_SIZE];
	char cmd_line[B_SIZE];
};

