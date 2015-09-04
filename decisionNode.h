/* Sudoku Solver
   decisionNode.h
   Sept 4, 2015
*/

#ifndef DNODE_H
#define DNODE_H

typedef struct DecisionNode {
  Cell *cell;
  struct DecisionNode *parent;
  int numBranches;
  int *branchVals;
  struct DecisionNode **branchChildren;
} DNode;

/*
 Decision Node functions
 */
DNode *createDNode(DNode *parent);
void destroyDNode(DNode *node);

void setDNodeCell(DNode *node, Cell *cell);

#endif
