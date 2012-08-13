#ifndef NODEADT_H_
#define NODEADT_H_

typedef struct nodeCDT * nodeADT;
nodeADT newNode(int instr);
void addCondList(nodeADT node, nodeADT condList);
void addNextList(nodeADT node, nodeADT nextList);
void addParam(nodeADT node, int param);


#endif /* NODEADT_H_ */
