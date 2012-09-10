#ifndef SERVERGENERIC_H_
#define SERVERGENERIC_H_

#include "parser.h"
#include "serializer.h"
#include "ipc.h"
#include "executor.h"

void programAttention(char * filepath, int pid);
void catchSignal(int signal);

#endif
