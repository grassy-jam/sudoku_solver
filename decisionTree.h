/* Sudoku Solver
   decisionTree.h
   Sept 4, 2015
*/

#ifndef DTREE_H
#define DTREE_H

typedef struct DecisionTree {
  DNode *curr;
  DNode *root;
} DTree;

/*
 Decision Tree functions
 */
DTree *createDTree();
void destroyDTree(DTree *tree);
void setDTreeCurr(DTree *tree, DNode *newCurr);
void setDTreeCurrToCell(DTree *tree, Cell *cell);

#endif
