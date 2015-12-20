/* Sudoku Solver
   cell.c
*/

#include "cell.h"

/*
  Cell manipulation functions
*/
Cell *createCell(int val, int row, int col) {
  Cell *cell;

  cell = malloc(sizeof(Cell));

  cell->val = val;
  cell->possibleValues = NULL;
  cell->length = 0;
  cell->row = row;
  cell->col = col;

  return cell;
}
void destroyCell(Cell *cell) {
  int i;

  if (cell->possibleValues != NULL) {
    free(cell->possibleValues);
  }
  free(cell);
}
