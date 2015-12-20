/* Sudoku Solver
   sudokuBoard.h
   Sept 4, 2015
*/

#ifndef SUDOKU_BOARD_H
#define SUDOKU_BOARD_H

#include "bool.h"
#include "cell.h"
#include "cellPriorityQueue.h"
#include "decisionTree.h"

typedef struct SudokuBoard {
  Cell ***board;
  int size;
  bool rowVectors[9][9];  //9 vectors of size 9, where each value in the vector represent whether the row contains the value at that index or not
  bool colVectors[9][9];  //9 vectors of size 9, where each value in the vector represent whether the col contains the value at that index or not
  bool ssVectors[9][9];
  CellPQueue *emptyCells;
} SBoard;

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
void initialFindAndPrune(SBoard *board);

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
/*
 Vector manipulation functions
 */
void setRowVector(SBoard *board, int row);
void setColVector(SBoard *board, int col);
void setSubSquareVector(SBoard *board, int ssI );
//This function will analyze the board and populate the row, col, and subsquare vectors based on the values already filled in the sudoku board
void setBoardVectors(SBoard *board);

bool *getSubSquareVector(SBoard *board, int row, int col);

#endif
