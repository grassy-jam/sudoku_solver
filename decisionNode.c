/* Sudoku Solver
   decisionNode.c
   Sept 4, 2015
*/

#include "decisionNode.h"

/*
 Decision Node functions
 */
DNode *createDNode(DNode *parent) {
  DNode *node;
  node = malloc(sizeof(DNode));
  
  node->cell = NULL;
  node->parent = parent;
  node->numBranches = 0;
  node->branchVals = NULL;
  node->branchChildren = NULL;
  
  return node;
}
//Recursively frees itself and children
void destroyDNode(DNode *node) {
  int i;
  for (i = 0; i < node->numBranches; i++) {
    destroyDNode(node->branchChildren[i]);
  }
  free(node->branchVals);
  free(node);
}

void setDNodeCell(DNode *node, Cell *cell) {
  int i;
  if (node == NULL) {
    node = createDNode(NULL);
  }
  node->cell = cell;
  node->numBranches = cell->length;
  if (node->branchVals != NULL) {
    free(node->branchVals);
  }
  node->branchVals = malloc(sizeof(int) * node->numBranches);
  for (i = 0; i < node->numBranches; i++) {
    node->branchVals[i] = cell->possibleValues[i];
  }
  if (node->branchChildren != NULL) {
    //node->numBranches may be different than was originally set, this is probably incorrect
    for (i = 0; i < node->numBranches; i++) {
      destroyDNode(node->branchChildren[i]);
    }
    free(node->branchChildren);
  }
  node->branchChildren = malloc(sizeof(DNode *) * node->numBranches);
  for (i = 0; i < node->numBranches; i++) {
    node->branchChildren[i] = createDNode(node);
  }
  printf("node cell is %p\n", node->cell);
}
