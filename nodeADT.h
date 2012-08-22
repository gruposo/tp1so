#ifndef NODEADT_H_
#define NODEADT_H_

typedef struct nodeCDT * nodeADT;
nodeADT newNode(int instr);
int getOperation(nodeADT node);

int getParam(nodeADT node);

nodeADT getNext(nodeADT node);

nodeADT getExe(nodeADT node);

void addParam(nodeADT node, int param);

void addReturn(nodeADT node, nodeADT returnTO);

void addExe(nodeADT node, nodeADT exeList);

void addNext(nodeADT node, nodeADT next);

void addJump(nodeADT node, nodeADT jump);

void print(nodeADT node);

#endif /* NODEADT_H_ */
