/* Sudoku Solver
   cell.h
*/

#ifndef CELL_H
#define CELL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Cell {
  int val;
  int *possibleValues;
  int length;  //length of possible values
  int row;
  int col;
} Cell;

/*
 Cell manipulation functions
 */
Cell *createCell(int val, int row, int col);
void destroyCell(Cell *cell);

#endif
