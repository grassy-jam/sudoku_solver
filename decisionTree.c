/* Sudoku Solver
   decisionTree.c
   Sept 4, 2015
*/

#include "decisionTree.h"

/*
 Decision Tree functions
 */
DTree *createDTree() {
  DTree *tree;
  
  tree = malloc(sizeof(DTree));
  
  tree->root = NULL;
  tree->curr = NULL;
  
  return tree;
}
void destroyDTree(DTree *tree) {
  if (tree->root != NULL) {
    destroyDNode(tree->root);
  }
  free(tree);
}
void setDTreeCurr(DTree *tree, DNode *newCurr) {
  if (tree->root == NULL) {
    tree->root = newCurr;
  }
  tree->curr = newCurr;
}
void setDTreeCurrToCell(DTree *tree, Cell *cell) {
  int i;
  DNode *node;
  
  node = tree->curr;
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
  tree->curr = node;
//  printf("node cell is %p\n", node->cell);
}
