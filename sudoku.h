/* Sudoku Solver
  April 2, 2015

  by Chris Grass and Jordon McKoy

  Will populate a sudoku board partially, and then solve it.
  This will print the initial board "problem" and then print the solved board (or simply say that there is no solution if there is none)

  example usage:
    > gcc sudoku.c -o sudokuSolver
    > ./sudokuSolver

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
  false = 0,
  true
} bool;

typedef struct Cell {
  int val;
  int *possibleValues;
  int length;  //length of possible values
  int row;
  int col;
} Cell;

typedef struct CellPriorityQueue {
  Cell **cells;
  int length;
  
} CellPQueue;

typedef struct SudokuBoard {
  Cell ***board;
  int size;
  bool rowVectors[9][9];  //9 vectors of size 9, where each value in the vector represent whether the row contains the value at that index or not
  bool colVectors[9][9];  //9 vectors of size 9, where each value in the vector represent whether the col contains the value at that index or not
  bool ssVectors[9][9];
  CellPQueue *emptyCells;
} SBoard;

typedef struct DecisionNode {
  Cell *cell;
  struct DecisionNode *parent;
  int numBranches;
  int *branchVals;
  struct DecisionNode **branchChildren;
} DNode;
typedef struct DecisionTree {
  DNode *curr;
  DNode *root;
} DTree;
/*
 Generic helpers
 */
//Returns a true the array contains the specificed num, and false otherwise
bool arrayContains(int* arrayOfSizeNine, int num);
bool isEmpty(int vals[9]);
//returns true if a value was found and removed from array
bool removeValFromArray(int *array, int n, int val);
/*
 Decision Node functions
 */
DNode *createDNode(DNode *parent);
void destroyDNode(DNode *node);

void setDNodeCell(DNode *node, Cell *cell);
/*
 Decision Tree functions
 */
DTree *createDTree();
void destroyDTree(DTree *tree);
void setDTreeCurr(DTree *tree, DNode *newCurr);
void setDTreeCurrToCell(DTree *tree, Cell *cell);
/*
 Cell manipulation functions
 */
Cell *createCell(int val, int row, int col);
void destroyCell(Cell *cell);
/*
 Cell Priority Queue functions
 */
CellPQueue *createCellPQueue();
//CellPQueue will have an array of cell pointers, only responsible for freeing that array, not the cell pointers themselves
void destroyCellPQueue(CellPQueue *pQueue);
Cell *removeFirstCell(CellPQueue *pQueue);
void appendLastCell(CellPQueue *pQueue, Cell *cell);
void sortCellPQueueByLength(CellPQueue *pQueue);
/*
 Board manipulation functions
 */
SBoard *createBoard(int n);
void *destroyBoard(SBoard *board);

bool rowHasOneMissingValue(SBoard *board, int row);
bool colHasOneMissingValue(SBoard *board, int col);
bool rowIsFull(SBoard *board, int row);
bool colIsFull(SBoard *board, int col);

int getCellValue(SBoard *board, int row, int col);
int *getRowValues(SBoard *board, int row);
int *getColValues(SBoard *board, int col);
int *getSubSquareValues(SBoard *board, int row, int col);

void setCellValue(SBoard *board, int row, int col, int val);
void setRowValues(SBoard *board, int row, int rowVals[9]);
void setColValues(SBoard *board, int col, int colVals[9]);
void setSubSquareValues(SBoard *board, int row, int col, int ssVals[9]);

//Finding all of the possible values for a cell based on the board's row, col, and ss vectors
void crossCellVectors(SBoard *board, int row, int col);
void setPossibleCellValues(SBoard *board);
void resetPossibleCellValues(SBoard *board, int row, int col);
bool fillSingleChoiceCells(SBoard *board);
void initialFindAndPrine(SBoard *board);

void setEmptyCells(SBoard *board);
bool solveEmptyCells(SBoard *board, DTree *dTree);

bool checkPossibleMove(SBoard *board, Cell *cell, int val);
bool solveCell(SBoard *board, int row, int col);
//Returns true if the board was successfully solved, and false if unsolvable
bool solveBoard(SBoard *board);

//fills the board with a generic solution, irregardless of what was in the cells
void fillFull(SBoard *board);
//fills the board partially, with a "problem" read from input.sudoku
void fillPartial(SBoard *board, char line[10], int index);
//fills the board with the example given in the assignment. Used for testing
void fillExample(SBoard *board);
/*
 Print functions
 */
void printRowVectors(SBoard *board);
void printColVectors(SBoard *board);
void printSubSquareVectors(SBoard *board);
void printBoard(SBoard *board);
void printPQueue(CellPQueue *pQueue);
/*
 Vector manipulation functions
 */
void setRowVector(SBoard *board, int row);
void setColVector(SBoard *board, int col);
void setSubSquareVector(SBoard *board, int ssI );
//This function will analyze the board and populate the row, col, and subsquare vectors based on the values already filled in the sudoku board
void setBoardVectors(SBoard *board);

bool *getSubSquareVector(SBoard *board, int row, int col);